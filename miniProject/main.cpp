// O------------------------------------------------------------------------------O
// | Example "Hello World" Program (main.cpp)                                     |
// O------------------------------------------------------------------------------O

#include <iostream>
#include <stack>
#include <queue>
#include <vector>
#include <time.h>
#include <cstdlib>
#include <cmath>



#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

using olc::vf2d;
using olc::Pixel;
using std::vector;
using std::stack;
using std::queue;




class Window : public olc::PixelGameEngine
{
public:
	Window()
	{
		// Name your application
		sAppName = "Maze Game";
		scene;
	}
private:
	int scale;
	
	float FPS = 0.00375;
	float FPSX = 3;// 16.299;

	int startX = 0;
	int startY = 0;
	
	int endX = 0;
	int endY = 0;

	int bfsDisplay = 0;
	bool bfsFound = false;

	float animationDelay = 0;

	
	int mazeW = 15;
	int mazeH = 15;

	bool update = true;

	int mazeRealW()
	{
		return mazeW + 2;
	}
	int mazeRealH()
	{
		return mazeH + 2;
	}

	int mazeOffsetX = 0;
	int mazeOffsetY = 0;

	int centerScreenW = ScreenWidth() / 2;
	int centerScreenH = ScreenHeight() / 2;

	vector<vector<int>> maze;

	vector<vector<int>> mazeDisplay;

	queue<vector<int>> mazeQCreate;

	queue<vector<int>> qSolve;

	enum scene
	{
		menue = 1,
		generateMaze = 2,
		solveMaze = 3,
		mazeSize = 4,
	};

	enum solve
	{
		BFS = 1,
		DFS = 2,
	};

	enum generate
	{
		dfsG = 1,
		primsG = 2,
		blobG = 3
	};

	generate generate;

	solve solve;

	scene scene;

public:
	bool OnUserCreate() override
	{
		generate = dfsG;

		solve = BFS;

		scene = menue;

		scale = std::min((ScreenWidth() - 22) / (mazeW + 2), (ScreenHeight() - 22) / (mazeH + 2));
		
		return true;
	}
	bool OnUserUpdate(float fElapsedTime) override
	{
		if (GetMouse(0).bHeld)
		{
			if (GetMouseX() > ScreenWidth() * 7 / 10 && GetMouseX() < ScreenWidth() && GetMouseY() > 2 && GetMouseY() < 10)
			{
				scene = menue;
				update = true;

				clear(maze);
				clear(mazeDisplay);
				clear(mazeQCreate);
				clear(qSolve);
				
			}
		}

		if (GetMouseWheel() > 0)
		{
			FPSX++;
		}
		if (GetMouseWheel() < 0)
		{
			FPSX--;
		}
		if (FPSX < 0)
			FPSX = 0;
		FPS = 1 / (FPSX * FPSX + 1);

		FillRect(ScreenWidth() * 3 / 10, 2, ScreenWidth() * 4 / 10, 8, olc::DARK_BLUE);
		DrawString(ScreenWidth() * 3 / 10, 2, "FPS: " + std::to_string(FPS), olc::YELLOW);



		if (update) //if update needed
		{
			Clear(olc::DARK_BLUE);

			// Draw Boundary
			DrawLine(10, 10, ScreenWidth() - 10, 10, olc::YELLOW);
			DrawLine(10, 10, 10, ScreenHeight() - 10, olc::YELLOW);
			DrawLine(ScreenWidth() - 10, 10, ScreenWidth() - 10, ScreenHeight() - 10, olc::YELLOW);

			DrawLine(10, ScreenHeight() - 10, ScreenWidth() - 10, ScreenHeight() - 10, olc::YELLOW);

			//draw text
			DrawString(ScreenWidth() * 7 / 10, 2, "Back to Menue", olc::YELLOW);

			update = false;
		}

		
		
		switch (scene)
		{
			case menue:
			{
				
				//current slide
					DrawString(15, 2, "Menue", olc::YELLOW);

				//maze height and width
					DrawString(ScreenWidth() / 2 - ScreenWidth() / 4, ScreenHeight() / 2 - ScreenHeight() / 4, "Change Maze Size: H:" + std::to_string(mazeH) + " W:" + std::to_string(mazeW), olc::YELLOW);

					if (GetMouse(0).bHeld)
					{
						if (GetMouseX() > ScreenWidth() / 2 - ScreenWidth() / 4 - 2 &&
							GetMouseX() < ScreenWidth() / 2 + ScreenWidth() / 4 + 2 &&
							GetMouseY() > ScreenHeight() / 2 - ScreenHeight() / 4 - 2 &&
							GetMouseY() < ScreenHeight() / 2 - ScreenHeight() / 4 + 14)
						{
							scene = mazeSize;
							update = true;
							FPSX = 4.359;
						}
					}
			
				//menue generate
					DrawString(20, ScreenHeight() / 2 - ScreenHeight() / 4 + 20, "Generate DFS", olc::YELLOW);

					//box
						//DrawRect(15, ScreenHeight() / 2 - ScreenHeight() / 4 + 18, ScreenWidth() / 2 - 40, 12, olc::YELLOW);
			
					DrawString(20, ScreenHeight() / 2 - ScreenHeight() / 4 + 35, "Generate Prims", olc::YELLOW);

					//box
						//DrawRect(15, ScreenHeight() / 2 - ScreenHeight() / 4 + 33, ScreenWidth() / 2 - 40, 12, olc::YELLOW);
			
					DrawString(20, ScreenHeight() / 2 - ScreenHeight() / 4 + 50, "Generate Blob", olc::YELLOW);

				//menue solve
			
					DrawString(ScreenWidth() / 2 + 20, ScreenHeight() / 2 - ScreenHeight() / 4 + 25, "Solve BFS", olc::YELLOW);
			
					DrawString(ScreenWidth() / 2 + 20, ScreenHeight() / 2 - ScreenHeight() / 4 + 45, "Solve DFS", olc::YELLOW);

				//box around solve method
					switch (solve)
					{
						case BFS:

							DrawLine(ScreenWidth() / 2 + 15, ScreenHeight() / 2 - ScreenHeight() / 4 + 20, ScreenWidth() / 2 + 95, ScreenHeight() / 2 - ScreenHeight() / 4 + 20, olc::YELLOW); //top bar

							DrawLine(ScreenWidth() / 2 + 15, ScreenHeight() / 2 - ScreenHeight() / 4 + 20, ScreenWidth() / 2 + 15, ScreenHeight() / 2 - ScreenHeight() / 4 + 38, olc::YELLOW); //left bar

							DrawLine(ScreenWidth() / 2 + 95, ScreenHeight() / 2 - ScreenHeight() / 4 + 20, ScreenWidth() / 2 + 95, ScreenHeight() / 2 - ScreenHeight() / 4 + 38, olc::YELLOW); //right bar

							DrawLine(ScreenWidth() / 2 + 15, ScreenHeight() / 2 - ScreenHeight() / 4 + 38, ScreenWidth() / 2 + 95, ScreenHeight() / 2 - ScreenHeight() / 4 + 38, olc::YELLOW); //botom bar
				
							break;
						case DFS:
							DrawLine(ScreenWidth() / 2 + 15, ScreenHeight() / 2 - ScreenHeight() / 4 + 40, ScreenWidth() / 2 + 95, ScreenHeight() / 2 - ScreenHeight() / 4 + 40, olc::YELLOW); //top bar

							DrawLine(ScreenWidth() / 2 + 15, ScreenHeight() / 2 - ScreenHeight() / 4 + 40, ScreenWidth() / 2 + 15, ScreenHeight() / 2 - ScreenHeight() / 4 + 58, olc::YELLOW); //left bar

							DrawLine(ScreenWidth() / 2 + 95, ScreenHeight() / 2 - ScreenHeight() / 4 + 40, ScreenWidth() / 2 + 95, ScreenHeight() / 2 - ScreenHeight() / 4 + 58, olc::YELLOW); //right bar

							DrawLine(ScreenWidth() / 2 + 15, ScreenHeight() / 2 - ScreenHeight() / 4 + 58, ScreenWidth() / 2 + 95, ScreenHeight() / 2 - ScreenHeight() / 4 + 58, olc::YELLOW); //botom bar
				
							break;
					}

				if (GetMouse(0).bHeld)
				{

					// select solve type
					if (GetMouseX() > ScreenWidth() / 2 + 15 && GetMouseX() < ScreenWidth() / 2 + 95 && GetMouseY() > ScreenHeight() / 2 - ScreenHeight() / 4 + 20 && GetMouseY() < ScreenHeight() / 2 - ScreenHeight() / 4 + 38)
					{
						solve = BFS;
						update = true;
					}
					if (GetMouseX() > ScreenWidth() / 2 + 15 && GetMouseX() < ScreenWidth() / 2 + 95 && GetMouseY() > ScreenHeight() / 2 - ScreenHeight() / 4 + 40 && GetMouseY() < ScreenHeight() / 2 - ScreenHeight() / 4 + 58)
					{
						solve = DFS;
						update = true;
					}

					//generate maze

					if (GetMouseX() > 15 && GetMouseX() < ScreenWidth() / 2 - 25)
					{
						if (GetMouseY() > ScreenHeight() / 2 - ScreenHeight() / 4 + 18 && GetMouseY() < ScreenHeight() / 2 - ScreenHeight() / 4 + 30)
						{
							generate = dfsG;
							scene = generateMaze;
							update = true;
							dfsMaze(maze, mazeQCreate);
							FPSX = 3;// 16.299;
						}
						if (GetMouseY() > ScreenHeight() / 2 - ScreenHeight() / 4 + 33 && GetMouseY() < ScreenHeight() / 2 - ScreenHeight() / 4 + 45)
						{
							generate = primsG;
							update = true;
							scene = generateMaze;
							primsMaze(maze, mazeQCreate);
							FPSX = 3;// 16.299;
						}
						if (GetMouseY() > ScreenHeight() / 2 - ScreenHeight() / 4 + 48 && GetMouseY() < ScreenHeight() / 2 - ScreenHeight() / 4 + 60)
						{
							generate = blobG;
							update = true;
							//scene = generateMaze;
							FPSX = 3;// 16.299;
						}
					}
				}
			
				break;
			}
			case generateMaze:
			{
				//Continue to solving
				DrawString(ScreenWidth() * 6 / 10, ScreenHeight() - 8, "Continue to solving", olc::YELLOW);

				if (GetMouse(0).bHeld)
					if (GetMouseX() > ScreenWidth() * 6 / 10 && GetMouseX() < ScreenWidth() && GetMouseY() > ScreenHeight() - 8 && GetMouseY() < ScreenHeight())
					{
						scene = solveMaze;

						vector<int> temp = genValidPoint(maze);

						startX = temp[0];
						startY = temp[1];
						
						temp = genValidPoint(maze);

						endX = temp[0];
						endY = temp[1];

						if (solve == BFS)
						{
							bfsDisplay = 0;
							bfsFound = false;

							FPSX = 1;
							
							vector<vector<int>> del;
							std::swap(mazeDisplay, del);
							
							std::copy(maze.begin(), maze.end(), back_inserter(mazeDisplay));

							mazeSolveBFS(mazeDisplay, qSolve, startX, startY, endX, endY);
						}
						else if (solve == DFS)
						{
							bfsDisplay = 0;
							bfsFound = false;

							FPSX = 1;

							vector<vector<int>> del;
							std::swap(mazeDisplay, del);

							std::copy(maze.begin(), maze.end(), back_inserter(mazeDisplay));

							mazeSolveDFS(mazeDisplay, qSolve, startX, startY, endX, endY);
						}


						update = true;
					}

				//generate the maze
				DrawString(15, 2, "Generate", olc::YELLOW);

				mazeOffsetX = ScreenWidth() / 2 - (maze[0].size()) * scale / 2;
				mazeOffsetY = ScreenHeight() / 2 - (maze.size()) * scale / 2;

				

				animationDelay += fElapsedTime;
				if (animationDelay > FPS)
				{
					for (int i = 0; i < mazeDisplay.size(); i++)
					{
						for (int j = 0; j < mazeDisplay[0].size(); j++)
						{
							if (mazeDisplay[i][j] == 0)
								FillRect(mazeOffsetX + j * scale, mazeOffsetY + i * scale, 1 * scale, 1 * scale, olc::BLACK);
							else if (mazeDisplay[i][j] % 2 == 1 && mazeDisplay[i][j] != 1)
							{
								FillRect(mazeOffsetX + j * scale, mazeOffsetY + i * scale, 1 * scale, 1 * scale, olc::GREEN);
								mazeDisplay[i][j] -= 2;
							}
							else if (mazeDisplay[i][j] % 2 == 0)
							{
								FillRect(mazeOffsetX + j * scale, mazeOffsetY + i * scale, 1 * scale, 1 * scale, olc::RED);
								mazeDisplay[i][j] -= 2;
							}
							else
								FillRect(mazeOffsetX + j * scale, mazeOffsetY + i * scale, 1 * scale, 1 * scale, olc::WHITE);
						}
					}

					if (!mazeQCreate.empty())
					{
						vector<int> temp = mazeQCreate.front();
						mazeQCreate.pop();

						if (maze[temp[0]][temp[1]] == 1) //add to show maze, and display green
						{
							FillRect(mazeOffsetX + temp[1] * scale, mazeOffsetY + temp[0] * scale, 1 * scale, 1 * scale, olc::GREEN);
							mazeDisplay[temp[0]][temp[1]] = 9;
						}
						else //display red
						{
							FillRect(mazeOffsetX + temp[1] * scale, mazeOffsetY + temp[0] * scale, 1 * scale, 1 * scale, olc::RED);
							mazeDisplay[temp[0]][temp[1]] = 8;
						}
					}
					animationDelay = 0;
				}


				break;
			}
			case mazeSize:
			{
				DrawString(15, 2, "Change Maze", olc::YELLOW);

				DrawString(ScreenWidth() * 35 / 100, ScreenHeight() / 2 - ScreenHeight() / 4, "Change Maze Size", olc::YELLOW);

				DrawString(20, ScreenHeight() / 2 - ScreenHeight() / 4 + 20, "Height = " + std::to_string(mazeH) + "\t\tup/down to change", olc::YELLOW);

				DrawString(20, ScreenHeight() / 2 - ScreenHeight() / 4 + 35, "Width  = " + std::to_string(mazeW) + "\t\tleft/right to change", olc::YELLOW);

				DrawString(20, ScreenHeight() / 2 - ScreenHeight() / 4 + 50, "Scale  = " + std::to_string(scale), olc::YELLOW);


				animationDelay += fElapsedTime;
				if (animationDelay > FPS)
				{
					update = true;

					if (GetKey(olc::Key::LEFT).bHeld) mazeW = upToTheMaxSize(abs(mazeW - (1)), ScreenWidth() - 22);
					if (GetKey(olc::Key::RIGHT).bHeld) mazeW = upToTheMaxSize(abs(mazeW + 1), ScreenWidth() - 22);

					if (GetKey(olc::Key::UP).bHeld) mazeH = upToTheMaxSize(abs(mazeH + (1)), ScreenHeight() - 22);
					if (GetKey(olc::Key::DOWN).bHeld) mazeH = upToTheMaxSize(abs(mazeH - (1)), ScreenHeight() - 22);

					scale = std::min((ScreenWidth() - 22) / (mazeW + 2), (ScreenHeight() - 22) / (mazeH + 2));
					
					scale = scale < 1 ? 1 : scale;

					animationDelay = 0;
				}

				break;
			}
			case solveMaze:
			{
				DrawString(ScreenWidth() / 10, 2, "Solve", olc::YELLOW);

				mazeOffsetX = (ScreenWidth() - (maze[0].size()) * scale) / 2;
				mazeOffsetY = (ScreenHeight() - (maze.size()) * scale) / 2;

				animationDelay += fElapsedTime;
				if (animationDelay > FPS)
				{
					for (int i = 0; i < mazeDisplay.size(); i++)
					{
						for (int j = 0; j < mazeDisplay[0].size(); j++)
						{
							//std::cout << mazeDisplay[i][j] << " ";
							if (!qSolve.empty() && bfsDisplay == (qSolve.front())[2])
								bfsFound = true;
							if ((i == startX && j == startY) || (i == endX && j == endY))
							{
								FillRect(mazeOffsetX + j * scale, mazeOffsetY + i * scale, 1 * scale, 1 * scale, bfsDisplay % 2 ? olc::CYAN : olc::Pixel(255, 127, 0));
							}
							else if(mazeDisplay[i][j] == 2)
							{
								FillRect(mazeOffsetX + j * scale, mazeOffsetY + i * scale, 1 * scale, 1 * scale, olc::GREEN);
							}
							else if (mazeDisplay[i][j] == 0)
							{
								FillRect(mazeOffsetX + j * scale, mazeOffsetY + i * scale, 1 * scale, 1 * scale, olc::BLACK);
							}
							else if (mazeDisplay[i][j] == 1 || mazeDisplay[i][j] < bfsDisplay)
							{
								FillRect(mazeOffsetX + j * scale, mazeOffsetY + i * scale, 1 * scale, 1 * scale, olc::WHITE);
							}
							else if (mazeDisplay[i][j] > bfsDisplay)
							{
								float t = (float)(mazeDisplay[i][j] - bfsDisplay);//Gradient
								FillRect(mazeOffsetX + j * scale, mazeOffsetY + i * scale, 1 * scale, 1 * scale, olc::Pixel(255, (int)(180.0 * t / (t + std::min(mazeH, mazeW))), (int)(180.0 * t / (t + std::min(mazeH, mazeW))) ));//(255, 0, 0, 255 / (((bfsDisplay)-(mazeDisplay[i][j]) - 1))));// / mazeDisplay[i][j] + 1), 255 / (((bfsDisplay) - (mazeDisplay[i][j]) - 1) / mazeDisplay[i][j] + 1)));//RED);//Pixel(255, 255 - (bfsDisplay - mazeDisplay[i][j]), 255 - (bfsDisplay - mazeDisplay[i][j])));// FUN ((float)255 * (float)mazeDisplay[i][j]) / (float)bfsDisplay, ((float)255 * (float)mazeDisplay[i][j]) / (float)bfsDisplay)); // for fun change the last two to			
							}
							else if (mazeDisplay[i][j] == bfsDisplay)
							{
								FillRect(mazeOffsetX + j * scale, mazeOffsetY + i * scale, 1 * scale, 1 * scale, olc::DARK_RED);
							}
						}
						//std::cout << std::endl;
					}
					
					//std::cout << std::endl << std::endl << std::endl;
					
					if (!bfsFound)bfsDisplay--;
					
					else
					{
						if(!qSolve.empty())
						{
							vector<int> temp = qSolve.front();
							qSolve.pop();
							
							//std::cout << temp[0] << " " << temp[1] << std::endl;

							mazeDisplay[temp[0]][temp[1]] = 2;
						}
					}
					animationDelay = 0;
				}

				break;
			}
		}
		/*// Called once per frame, draws random coloured pixels
		for (int x = 0; x < ScreenWidth(); x++)
			for (int y = 0; y < ScreenHeight(); y++)
				Draw(x, y, olc::Pixel(rand() % 256, rand() % 256, rand() % 256));
		return true;*/

		return true;
	}












	vector<int>genValidPoint(vector<vector<int>>& maze)
	{

		vector<int> temp = { 0, 0 };

			while(maze[temp[0]][temp[1]] != 1)
				temp = { rand() % mazeH + 1 , rand() % mazeW + 1 };
		return temp;
	}

	int upToTheMaxSize(int num, int max)
	{
		return num > max ? max : num < 1 ? 1 : num;
	}


	bool surrounded(vector<vector<int>>& maze, int startX, int startY)
	{
		int surr = 0;

		surr += maze[startX - 1][startY];
		surr += maze[startX + 1][startY];
		surr += maze[startX][startY - 1];
		surr += maze[startX][startY + 1];

		return (surr < 2);
	}


	void dfsMaze(vector<vector<int>>& maze, queue<vector<int>>& q)
	{
		srand(time(NULL));

		vector<vector<int>> delMaze(mazeH + 2, vector<int>(mazeW + 2, 0));

		vector<vector<int>> delMaze2(mazeH + 2, vector<int>(mazeW + 2, 0));

		swap(mazeDisplay, delMaze2);

		swap(maze, delMaze);

		int startX = rand() % mazeH + 1;
		int startY = rand() % mazeW + 1;

		vector<int> shift = { 0,1,0,-1,0, 0, 0, 0 };

		int height = maze.size();
		int width = maze[0].size();

		stack<vector<int>> s;

		s.push({ startX, startY });

		while (!s.empty())
		{
			vector<int> temp = s.top();
			s.pop();
			q.push(temp);

			if (temp[0] > 0 && temp[0] < height - 1 && temp[1] > 0 && temp[1] < width - 1 && maze[temp[0]][temp[1]] == 0)  //check that its withinn the border of 0
			{
				if (surrounded(maze, temp[0], temp[1]))
				{
					maze[temp[0]][temp[1]] = 1;
					for (int i = 0; i < 1111;)
					{
						int rShift = rand() % 4;

						//std::cout << "[" << rShift << " " << i << "]    ";

						if ((i / (int)pow(10, rShift)) % 10 == 0)
						{
							i += (int)pow(10, rShift);
							s.push({ temp[0] + shift[rShift], temp[1] + shift[rShift + 1] });

							//std::cout << "+" << (int)pow(10, rShift) << "=" << i << std::endl;

						}

					}
				}

			}
		}
	}


	void primsMaze(vector<vector<int>>& maze, queue<vector<int>>& q)
	{
		srand(time(NULL));

		vector<vector<int>> delMaze(mazeH + 2, vector<int>(mazeW + 2, 0));

		vector<vector<int>> delMaze2(mazeH + 2, vector<int>(mazeW + 2, 0));

		swap(mazeDisplay, delMaze2);

		swap(maze, delMaze);

		int startX = rand() % mazeH + 1;
		int startY = rand() % mazeW + 1;

		vector<int> shift = { 0,1,0,-1,0, 0, 0, 0 };

		int height = maze.size();
		int width = maze[0].size();

		vector<vector<int>> s;

		s.push_back({ startX, startY });

		while (!s.empty())
		{
			int index = rand() % s.size();
			vector<int> temp = s[index];
			std::swap(s.back(), s[index]);
			s.pop_back();
			q.push(temp);

			if (temp[0] > 0 && temp[0] < height - 1 && temp[1] > 0 && temp[1] < width - 1 && maze[temp[0]][temp[1]] == 0)  //check that its withinn the border of 0
			{
				if (surrounded(maze, temp[0], temp[1]))
				{
					maze[temp[0]][temp[1]] = 1;
					
					for (int i = 0; i < 1111;)
					{
						int rShift = rand() % 4;

						//std::cout << "[" << rShift << " " << i << "]    ";

						if ((i / (int)pow(10, rShift)) % 10 == 0)
						{
							i += (int)pow(10, rShift);
							s.push_back({ temp[0] + shift[rShift], temp[1] + shift[rShift + 1] });

							//std::cout << "+" << (int)pow(10, rShift) << "=" << i << std::endl;

						}

					}
					
					/*for (int i = 0; i < 4; i++)
					{
						s.push_back({ temp[0] + shift[i], temp[1] + shift[i + 1]});
					}*/
				}

			}
		}
	}
	


	

	void clear(std::queue<vector<int>>& q)
	{
		std::queue<vector<int>> empty;
		std::swap(q, empty);
	}
	
	void clear(std::stack<vector<int>>& q)
	{
		std::stack<vector<int>> empty;
		std::swap(q, empty);
	}
	
	void clear(std::vector<vector<int>>& q)
	{
		std::vector<vector<int>> empty;
		std::swap(q, empty);
	}

	void mazeSolveBFS(vector<vector<int>>& maze, queue<vector<int>>& q, int startX, int startY, int endX, int endY)
	{
		vector<int> shift = { 0,1,0,-1,0, 0, 0, 0 };

		int height = maze.size();
		int width = maze[0].size();

		queue<vector<int>> s;

		s.push({ startX, startY, -1 });

		while (!s.empty())
		{
			vector<int> temp = s.front();
			s.pop();

			if (temp[0] == endX && temp[1] == endY)
			{
				clear(s);
				maze[temp[0]][temp[1]] = temp[2];
			}
			else if (temp[0] > 0 && temp[0] < height - 1 && temp[1] > 0 && temp[1] < width - 1 && maze[temp[0]][temp[1]] == 1)
			{
				maze[temp[0]][temp[1]] = temp[2];

				for (int i = 0; i < 4; i++)
				{
					s.push({ temp[0] + shift[i], temp[1] + shift[i + 1], temp[2] - 1 });
				}
			}
		}


		int curentValue = maze[endX][endY];

		if (curentValue == 1 || curentValue == 0)
			std::cout << "unsolvable maze /\n\n";

		else
		{
			q.push({ endX, endY, curentValue});

			while (curentValue < 0)
			{
				for (int i = 0; i < 4; i++)
				{
					if (curentValue == -1)
						curentValue++;
					if (maze[endX + shift[i]][endY + shift[i + 1]] < 0 && maze[endX + shift[i]][endY + shift[i + 1]] > curentValue)
					{
						endX += shift[i];
						endY += shift[i + 1];

						curentValue = maze[endX][endY];

						q.push({ endX, endY, curentValue});
					}
				}

			}

			q.push({ startX, startY, curentValue });
		}
	}

	
	void mazeSolveDFS(vector<vector<int>>& maze, queue<vector<int>>& q, int startX, int startY, int endX, int endY)
	{
		vector<int> shift = { 0,1,0,-1, 0, 0, 0, 0 };

		int height = maze.size();
		int width = maze[0].size();

		stack<vector<int>> s;

		s.push({ startX, startY });

		int curentValue = 0;

		while (!s.empty())
		{
			vector<int> temp = s.top();
			s.pop();

			//std::cout << "int the while loop" << temp[0] << " " << temp[1] << " " << curentValue << std::endl;

			if (temp[0] == endX && temp[1] == endY)
			{
				//std::cout << "found the end" << std::endl;
				clear(s);
				maze[temp[0]][temp[1]] = --curentValue;
			}
			else if (temp[0] > 0 && temp[0] < height - 1 && temp[1] > 0 && temp[1] < width - 1 && maze[temp[0]][temp[1]] == 1)
			{
				maze[temp[0]][temp[1]] = --curentValue;

				for (int i = 0; i < 4; i++)
				{
					s.push( {temp[0] + shift[i], temp[1] + shift[i + 1]} );
					//std::cout << "pushed " << temp[0] + shift[i] << " " << temp[1] + shift[i + 1] << std::endl;
				}
			}
		}

		//std::cout << "exited" << std::endl;

		curentValue = maze[endX][endY];

		if (curentValue == 1 || curentValue == 0)
			std::cout << "unsolvable maze /\n\n";

		else
		{
			q.push({ endX, endY, curentValue });

			while (curentValue < 0)
			{
				for (int i = 0; i < 4; i++)
				{
					//std::cout << endX << " " << endY << std::endl;
					if (curentValue == -1)
						curentValue++;
					if (maze[endX + shift[i]][endY + shift[i + 1]] < 0 && maze[endX + shift[i]][endY + shift[i + 1]] > curentValue)
					{
						endX += shift[i];
						endY += shift[i + 1];

						curentValue = maze[endX][endY];

						q.push({ endX, endY, curentValue });
					}
				}

			}

			q.push({ startX, startY, curentValue });
		}
	}
};

#include "wtypes.h"

void GetDesktopResolution(int& horizontal, int& vertical)
{
	RECT desktop;
	// Get a handle to the desktop window
	const HWND hDesktop = GetDesktopWindow();
	// Get the size of screen to the variable desktop
	GetWindowRect(hDesktop, &desktop);
	// The top left corner will have coordinates (0,0)
	// and the bottom right corner will have coordinates
	// (horizontal, vertical)
	horizontal = desktop.right;
	vertical = desktop.bottom;
}


int main()
{

	int horizontal = 0;
	int vertical = 0;
	GetDesktopResolution(horizontal, vertical);

	Window demo;
	if (demo.Construct(horizontal / 5, vertical / 5, 4, 4))
		demo.Start();
	return 0;
}