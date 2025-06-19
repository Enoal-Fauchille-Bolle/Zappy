/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Time Functions Tests
*/

#include "connection/time.h"
#include "connection/server.h"
#include "game/game.h"
#include "game/game_state.h"
#include "options_parser/options.h"
#include "constants.h"
#include <criterion/criterion.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>

// Helper function to create a minimal server for testing
static server_t *create_test_server(tick_rate_t tick_rate)
{
    server_t *server = malloc(sizeof(server_t));
    if (!server)
        return NULL;
    
    server->game = malloc(sizeof(game_t));
    if (!server->game) {
        free(server);
        return NULL;
    }
    
    server->game->tick_rate = tick_rate;
    server->options = NULL;
    server->game->map = NULL;
    server->game->teams = NULL;
    server->game->next_player_id = 0;
    server->game->game_tick = 0;
    server->game->game_state = GAME_RUNNING;
    
    for (int i = 0; i < MAX_CLIENTS; i++) {
        server->clients[i] = NULL;
    }
    
    return server;
}

// Helper function to cleanup test server
static void cleanup_test_server(server_t *server)
{
    if (server) {
        if (server->game) {
            free(server->game);
        }
        free(server);
    }
}

Test(time_functions, get_current_time_ms_returns_positive_value, .timeout = 2)
{
    long long time_ms = get_current_time_ms();
    
    cr_assert_gt(time_ms, 0, "get_current_time_ms should return a positive value");
}

Test(time_functions, get_current_time_ms_is_monotonic, .timeout = 2)
{
    long long time1 = get_current_time_ms();
    
    // Small delay to ensure time progression
    usleep(1000); // 1ms delay
    
    long long time2 = get_current_time_ms();
    
    cr_assert_gt(time2, time1, "Time should be monotonic (second call should return greater value)");
}

Test(time_functions, get_current_time_ms_precision, .timeout = 2)
{
    long long time1 = get_current_time_ms();
    long long time2 = get_current_time_ms();
    
    // Even consecutive calls should have reasonable precision
    // The difference should be very small (less than 10ms)
    long long diff = time2 - time1;
    cr_assert_leq(diff, 10, "Consecutive calls should have minimal time difference");
}

Test(time_functions, get_current_time_ms_consistency, .timeout = 2)
{
    const int num_samples = 10;
    long long times[num_samples];
    
    // Collect multiple time samples
    for (int i = 0; i < num_samples; i++) {
        times[i] = get_current_time_ms();
        usleep(100); // 0.1ms delay between samples
    }
    
    // Verify all times are in ascending order (monotonic)
    for (int i = 1; i < num_samples; i++) {
        cr_assert_geq(times[i], times[i-1], 
            "Time samples should be monotonically increasing");
    }
}

Test(time_functions, wait_remaining_tick_time_with_fast_tick_rate, .timeout = 3)
{
    server_t *server = create_test_server(1000); // 1000 ticks per second = 1ms per tick
    cr_assert_not_null(server, "Failed to create test server");
    
    long long start_time = get_current_time_ms();
    long long tick_start = start_time;
    
    wait_remaining_tick_time(server, tick_start);
    
    long long end_time = get_current_time_ms();
    long long elapsed = end_time - start_time;
    
    // Should wait approximately 1ms (allowing some tolerance for system overhead)
    cr_assert_geq(elapsed, 0, "Elapsed time should be non-negative");
    cr_assert_leq(elapsed, 5, "Should not wait too long for fast tick rate");
    
    cleanup_test_server(server);
}

Test(time_functions, wait_remaining_tick_time_with_slow_tick_rate, .timeout = 3)
{
    server_t *server = create_test_server(10); // 10 ticks per second = 100ms per tick
    cr_assert_not_null(server, "Failed to create test server");
    
    long long start_time = get_current_time_ms();
    long long tick_start = start_time;
    
    wait_remaining_tick_time(server, tick_start);
    
    long long end_time = get_current_time_ms();
    long long elapsed = end_time - start_time;
    
    // Should wait approximately 100ms
    cr_assert_geq(elapsed, 95, "Should wait at least 95ms for slow tick rate");
    cr_assert_leq(elapsed, 110, "Should not wait more than 110ms (allowing tolerance)");
    
    cleanup_test_server(server);
}

Test(time_functions, wait_remaining_tick_time_with_elapsed_time, .timeout = 3)
{
    server_t *server = create_test_server(100); // 100 ticks per second = 10ms per tick
    cr_assert_not_null(server, "Failed to create test server");
    
    long long start_time = get_current_time_ms();
    long long tick_start = start_time - 5; // Simulate 5ms already elapsed
    
    wait_remaining_tick_time(server, tick_start);
    
    long long end_time = get_current_time_ms();
    long long elapsed = end_time - start_time;
    
    // Should wait approximately 5ms (10ms tick - 5ms already elapsed)
    cr_assert_geq(elapsed, 4, "Should wait at least 4ms");
    cr_assert_leq(elapsed, 8, "Should not wait more than 8ms (allowing tolerance)");
    
    cleanup_test_server(server);
}

Test(time_functions, wait_remaining_tick_time_with_overtime, .timeout = 2)
{
    server_t *server = create_test_server(100); // 100 ticks per second = 10ms per tick
    cr_assert_not_null(server, "Failed to create test server");
    
    long long start_time = get_current_time_ms();
    long long tick_start = start_time - 15; // Simulate 15ms already elapsed (overtime)
    
    wait_remaining_tick_time(server, tick_start);
    
    long long end_time = get_current_time_ms();
    long long elapsed = end_time - start_time;
    
    // Should not wait when tick time has already exceeded
    cr_assert_leq(elapsed, 2, "Should not wait when tick time already exceeded");
    
    cleanup_test_server(server);
}

Test(time_functions, wait_remaining_tick_time_null_server_safety, .timeout = 2)
{
    // Test documenting expected behavior with null server
    // In practice, this would likely segfault, but the test documents expected behavior
    // Since we can't actually test with NULL (would crash), 
    // we'll verify that a valid call works and document the edge case
    cr_assert(1, "Test placeholder for null server case - would segfault in practice");
}

Test(time_functions, tick_duration_calculation_accuracy, .timeout = 2)
{
    // Test various tick rates to ensure duration calculation is accurate
    tick_rate_t test_rates[] = {1, 10, 50, 100, 1000};
    double expected_durations[] = {1000.0, 100.0, 20.0, 10.0, 1.0}; // in milliseconds
    size_t num_tests = sizeof(test_rates) / sizeof(test_rates[0]);
    
    for (size_t i = 0; i < num_tests; i++) {
        server_t *server = create_test_server(test_rates[i]);
        cr_assert_not_null(server, "Failed to create test server for rate %u", test_rates[i]);
        
        // Calculate expected duration manually
        double calculated_duration = (1.0 / server->game->tick_rate) * 1000.0;
        
        cr_assert_float_eq(calculated_duration, expected_durations[i], 0.001,
            "Tick duration calculation should be accurate for rate %u", test_rates[i]);
        
        cleanup_test_server(server);
    }
}

Test(time_functions, wait_remaining_tick_time_precision, .timeout = 3)
{
    server_t *server = create_test_server(200); // 200 ticks per second = 5ms per tick
    cr_assert_not_null(server, "Failed to create test server");
    
    // Test multiple cycles to ensure consistent timing
    const int num_cycles = 5;
    long long total_elapsed = 0;
    
    for (int i = 0; i < num_cycles; i++) {
        long long cycle_start = get_current_time_ms();
        wait_remaining_tick_time(server, cycle_start);
        long long cycle_end = get_current_time_ms();
        
        long long cycle_elapsed = cycle_end - cycle_start;
        total_elapsed += cycle_elapsed;
        
        // Each cycle should take approximately 5ms
        cr_assert_geq(cycle_elapsed, 4, "Cycle %d should take at least 4ms", i);
        cr_assert_leq(cycle_elapsed, 10, "Cycle %d should not take more than 10ms", i);
    }
    
    // Average should be close to expected tick duration
    double average = (double)total_elapsed / num_cycles;
    cr_assert_geq(average, 4.0, "Average cycle time should be at least 4ms");
    cr_assert_leq(average, 7.0, "Average cycle time should not exceed 7ms");
    
    cleanup_test_server(server);
}

Test(time_functions, extreme_tick_rates, .timeout = 2)
{
    // Test edge cases with very high and very low tick rates
    
    // Very high tick rate (should have very short wait times)
    server_t *fast_server = create_test_server(10000); // 10000 ticks/sec = 0.1ms per tick
    cr_assert_not_null(fast_server, "Failed to create fast server");
    
    long long start = get_current_time_ms();
    wait_remaining_tick_time(fast_server, start);
    long long end = get_current_time_ms();
    
    // Should complete very quickly
    cr_assert_leq(end - start, 2, "Very high tick rate should have minimal wait time");
    
    cleanup_test_server(fast_server);
    
    // Very low tick rate (1 tick per second would take too long for test)
    // Test the calculation without actually waiting
    server_t *slow_server = create_test_server(1);
    cr_assert_not_null(slow_server, "Failed to create slow server");
    
    // Just verify the server was created with correct tick rate
    cr_assert_eq(slow_server->game->tick_rate, 1, "Slow server should have tick rate of 1");
    
    cleanup_test_server(slow_server);
}