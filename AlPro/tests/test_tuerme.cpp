#include <gtest/gtest.h>

#include "tuerme_von_Hanoi.h"

// Test: Erstellen mit Höhe 0 (keine Scheiben)
TEST(TowersOfHanoiTest, ZeroHeight) {
    TowersOfHanoi t(0);
    EXPECT_TRUE(t.is_game_state_legal());
    EXPECT_TRUE(t.solve(false));
    EXPECT_TRUE(t.is_game_state_legal());
}

// Test: Lösen mit 3 Scheiben
TEST(TowersOfHanoiTest, SolveThreeDisks) {
    TowersOfHanoi t(3);
    EXPECT_TRUE(t.is_game_state_legal());
    EXPECT_TRUE(t.solve(false));
    EXPECT_TRUE(t.is_game_state_legal());
}

// Test: setup_game setzt korrekt zurück und Lösen funktioniert
TEST(TowersOfHanoiTest, SetupAndSolve) {
    TowersOfHanoi t(5);
    t.setup_game(2);
    EXPECT_TRUE(t.is_game_state_legal());
    EXPECT_TRUE(t.solve(false));
    EXPECT_TRUE(t.is_game_state_legal());
}

