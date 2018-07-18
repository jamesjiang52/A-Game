#include "ActiveEffectClass.hpp"

ActiveEffect::ActiveEffect(std::string targetStat, int amount, int combatTurnsForActivation, int duration) {
    this->targetStat = targetStat;
    this->amount = amount;
    this->combatTurnsForActivation = combatTurnsForActivation;
    this->duration = duration;
}

bool ActiveEffect::operator==(const ActiveEffect &activeEffect) {
    bool boolStat = (targetStat == activeEffect.getTargetStat());
    bool boolAmount = (amount == activeEffect.getAmount());
    bool boolCombatTurns = (combatTurnsForActivation == activeEffect.getCombatTurnsForActivation());
    bool boolDuration = (duration == activeEffect.getDuration());
    return (boolStat && boolAmount && boolCombatTurns && boolDuration);
}

std::string ActiveEffect::getTargetStat() const {
    return targetStat;
}

int ActiveEffect::getAmount() const {
    return amount;
}

int ActiveEffect::getCombatTurnsForActivation() const {
    return combatTurnsForActivation;
}

int ActiveEffect::getDuration() const {
    return duration;
}
