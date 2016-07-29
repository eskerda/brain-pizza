#include "Zombie.h"

//
// Wandering
//

Zombie::StateWanderingType Zombie::StateWandering;

void Zombie::StateWanderingType::Enter(Zombie* z) {
    z->steering->WanderOn();
    z->steering->PursuitOff();
    z->animatedSprite()->SetAnimation("moving");
}

void Zombie::StateWanderingType::Update(Zombie* z, float dt) {

    if (z->Pos().Distance(z->target->Pos()) < 500) {
        z->ChangeState(StatePursuing);
    }

}

void Zombie::StateWanderingType::Exit(Zombie* z) {

}


//
// Pursuing
//

Zombie::StatePursuingType Zombie::StatePursuing;

void Zombie::StatePursuingType::Enter(Zombie* z) {
    z->steering->WanderOff();
    z->steering->PursuitOn(z->target);
    z->animatedSprite()->SetAnimation("moving");
}

void Zombie::StatePursuingType::Update(Zombie* z, float dt) {

    if (z->Pos().Distance(z->target->Pos()) < 100) {
        z->ChangeState(StateAttacking);
    }

}

void Zombie::StatePursuingType::Exit(Zombie* z) {

}


//
// Attacking
//

Zombie::StateAttackingType Zombie::StateAttacking;

void Zombie::StateAttackingType::Enter(Zombie* z) {
    z->steering->WanderOff();
    z->steering->PursuitOff();
    z->animatedSprite()->SetAnimation("attack");
    z->animatedSprite()->SetRepeat(false);
}

void Zombie::StateAttackingType::Update(Zombie* z, float dt) {
    if (z->animatedSprite()->isFinished()) {
        z->ChangeState(Zombie::StateWandering);
    }
}

void Zombie::StateAttackingType::Exit(Zombie* z) {
    z->animatedSprite()->SetRepeat(true);
}

