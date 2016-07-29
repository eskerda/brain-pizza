#include "ZombieScene.h"
#include "BNode.h"


#include "Zombie.h"

int worldSize = 60;

void ZombieScene::Enter()
{
    steps = 7;
    lives = 3;
    immunity = 3;

    Input::SetWindowCaption("Zombie pizza delivery attack turbo ultra in the night");
    Input::SetWindowSize(sf::Vector2u(1000,700));

    fase = 0;

    hearth = new Sprite("media/heart.png");
    hearth->setScale(0.4,0.4);

    timer.setFont(FontResMan::GetDefaultFont());
    timer.setPosition(35, 35);

    bike = new Bike();

    tiles = World::generate(worldSize, worldSize, steps);

    // tiles =
    // {
    //     {'a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a'},
    //     {'a',' ','r','r',' ',' ','a',' ','r','r','r','r','r','r',' ','a'},
    //     {'a',' ','r','r',' ',' ','a',' ','r','r','r','r','r','r',' ','a'},
    //     {'a',' ','r','r',' ',' ','a',' ','r','r','r','r','r','r',' ','a'},
    //     {'a',' ',' ',' ',' ',' ','a',' ',' ',' ',' ',' ',' ',' ',' ','a'},
    //     {'a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a'},
    //     {'a','r','r','r',' ',' ','a',' ','r','r','r','r','r',' ',' ','a'},
    //     {'a','r','r','r',' ',' ','a',' ','r','r','r','r','r',' ',' ','a'},
    //     {'a','r','r','r',' ',' ','a',' ','r','r','r','r','r',' ',' ','a'},
    //     {'a','r','r','r',' ',' ','a',' ',' ',' ',' ',' ',' ',' ',' ','a'},
    //     {'a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a'},
    // };

    shaderTileMap = new ZombieTileMap(tiles, "roads.png", 240);

    Bounds b = shaderTileMap->GetBoundsInPixels();

    std::cout << b << std::endl;


    int numZombies = 500;
    for (int i = 0; i < numZombies; i++) {
        new Zombie(RandInBounds(b, 10), b, bike);
    }
    new Zombie(Vector2D(300,300), b, bike);

}

void ZombieScene::Update(float dt)
{
    Input::ChangeZoomWithPlusAndMinus(0.3,dt);

    fase += dt;

    if (lives > 0)
    {
        AliveUpdate(dt);
    }
    else
    {
        DeadUpdate(dt);
    }

    if (Input::IsKeyJustPressed(Input::DEBUG_D)) {
        steps++;
        tiles = World::generate(worldSize, worldSize, steps);
        shaderTileMap->LoadTileMap(tiles);
    }

    if (Input::IsKeyJustPressed(Input::DEBUG_F) && steps > 0) {
        steps--;
        tiles = World::generate(worldSize, worldSize, steps);
        shaderTileMap->LoadTileMap(tiles);
    }
}

void ZombieScene::DeadUpdate(float dt)
{
    loser.Update(dt);
}

void ZombieScene::AliveUpdate(float dt)
{
    for(Zombie * z : SelfRegister<Zombie*>::getAll()) {
        z->Update(dt);
    }

    bike->Update(dt);

    //Input::SetCameraCenter(bike->Pos());
    Input::SetCameraCenter(bike->Pos()+Vector2D(15*sin(fase),20*cos(fase/2)));


//TODO: Fer zoom out relatiu a la velocitat

    static bool blue = false;
    if (Input::IsKeyJustPressed(Input::DEBUG_B)) {
        blue = !blue;
    }

    if (immunity <= 0) {
        //Check colisions with zombies
        for (Zombie* z : SelfRegister<Zombie*>::getAll()) {
            if (z->Pos().Distance(bike->Pos()) < 50) {
                immunity = 2;
                lives--;
                if (lives == 0) {
                    loser.ShowMessage("FUCKING LOSER");
                }
                break;
            }
        }
    } else {
        immunity -= dt;
    }

    if (Input::IsKeyJustPressed(Input::DEBUG_F1)) {
        lives++;
    }

}

void ZombieScene::Draw(sf::RenderTarget& rt)
{
    rt.clear(sf::Color(100,0,0));

    shaderTileMap->Draw(rt);

    bike->DrawStroke(rt);

    for(Zombie * z : SelfRegister<Zombie*>::getAll()) {
        z->Draw(rt);
    }

    if (immunity > 0) {
        if (((int)(immunity*5))%3) {
            bike->Draw(rt);
        }
    } else {
        bike->Draw(rt);
    }

    Input::StartGuiDraw();
    for (int i = 0; i < lives; i++) {
        hearth->setPosition(Input::GetWindowBounds().width - 120 - i*100, 35);
        rt.draw(*hearth);
    }
    if (lives <= 0) {
        loser.Draw(rt);
    }
    timer.setString("Time: " + ftos(fase));
    rt.draw(timer);
    Input::EndGuiDraw();

}

void ZombieScene::Exit()
{

    delete bike;
    delete hearth;
    SelfRegister<Zombie*>::destroyAll();

    Input::ResetCamera();

}

