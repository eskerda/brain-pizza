#include "ZombieScene.h"
#include "BNode.h"


void ZombieScene::Enter()
{
    steps = 1;
	lives = 3;
	immunity = 3;

    Input::SetWindowCaption("Zombie pizza delivery attack turbo ultra in the night");
    Input::SetWindowSize(sf::Vector2u(1000,700));

    fase = 0;

	hearth = new Sprite("media/heart.png");
	hearth->setScale(0.4,0.4);

    timer.setFont(FontResMan::GetDefaultFont());
	timer.setPosition(35, 35);

    ship = new Ship();

    tiles = World::generate(50, 50, steps);

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
        tiles = World::generate(50, 50, steps);
        shaderTileMap->LoadTileMap(tiles);
    }

    if (Input::IsKeyJustPressed(Input::DEBUG_F)) {
        steps--;
        tiles = World::generate(50, 50, steps);
        shaderTileMap->LoadTileMap(tiles);
    }
}

void ZombieScene::DeadUpdate(float dt)
{
    loser.Update(dt);
}

void ZombieScene::AliveUpdate(float dt)
{
    ship->Update(dt);

    //Input::SetCameraCenter(ship->Pos());
    Input::SetCameraCenter(ship->Pos()+Vector2D(15*sin(fase),20*cos(fase/2)));


//TODO: Fer zoom out relatiu a la velocitat

    static bool blue = false;
    if (Input::IsKeyJustPressed(Input::DEBUG_B)) {
        blue = !blue;
    }

    if (immunity <= 0) {
        //Check colisions with zombies
        /*
        for (unsigned int i = 0; i < zombies.GetUsedSize(); i++) {
            const Zombie& entity = zombies[i];
            if (entity.Pos().Distance(ship->Pos()) < 50) {
                immunity = 2;
                lives--;
                if (lives == 0) {
                    loser.ShowMessage("FUCKING LOSER");
                }
                break;
            }
        }
*/
    } else {
        immunity -= dt;
    }

    if (Input::IsKeyJustPressed(Input::DEBUG_F1)) {
        lives++;
    }

}

void ZombieScene::Draw(sf::RenderTarget& rt)
{
    rt.clear(sf::Color(0,162,232));

    shaderTileMap->Draw(rt);

	ship->DrawStroke(rt);

	if (immunity > 0) {
		if (((int)(immunity*5))%3) {
			ship->Draw(rt);
		}
	} else {
		ship->Draw(rt);
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

	delete ship;
	delete hearth;

	Input::ResetCamera();

}

