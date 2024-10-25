//
// Created by arch_ on 24-10-25.
//
#pragma once
class Firefly {
public:
    Firefly()
        : mood { 100 }
        , satiety { 100 }
        , stamina { 100 }
    {
    }
    void Mood(const int value) { mood = value; }
    void Satiety(const int value) { satiety = value; }
    void Stamina(const int value) { stamina = value; }
    [[nodiscard]] int Mood() const { return mood; }
    [[nodiscard]] int Satiety() const { return satiety; }
    [[nodiscard]] int Stamina() const { return stamina; }

private:
    int mood;
    int satiety;
    int stamina;
};