#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <time.h>
#include <iostream>
#include <string>

using namespace std;
using namespace sf;

const int BoardSize = 8;		//default 8
const int moveSpeed = 10;		//default 10

int tileSize = 55; //tile size
Vector2i offset(500, 50); //default 500,50

struct tile
{
	int xCord;
	int col;
	int yCord;
	int row;
	

	int kind;
	int isMatched;
	int alpha;

	tile() {
		isMatched = 0;
		alpha = 255;
		kind = -1;
		xCord = yCord = row = col = 0;
	}
};

//The Board to display the tiles will be a 2d array of tiles. 
//However, its size is 2 greater than the actual displayable board, meaning that it has a boundary of empty tiles all around it.
//This strategy allows to use less 'if' conditions to check if boundary reached etc. 

tile Board[BoardSize + 2][BoardSize + 2];

//function to swap the row and column of two tiles
void swap(tile p1, tile p2)
{
	int temp = p1.col;
	p1.col = p2.col;
	p2.col = temp;

	temp = p1.row;
	p1.row = p2.row;
	p2.row = temp;

	Board[p1.row][p1.col] = p1;
	Board[p2.row][p2.col] = p2;
}


//Make all tiles matchable
void clearBoard()
{
	int i = 1;
	int j = 1;
	while (i <= BoardSize) {
		j = 1;
		while (j <= BoardSize) {
			Board[i][j].isMatched = 1;
			j++;
		}
		i++;
	}
}

//Check if there are any remaining moves left on the board
bool checkMatchable(bool hint = 0) {
	int i = 1;
	int j = 1;
	bool matchFound = 0;

	//This loop checks each tile to see if it has a similar tile in front of it,two steps in front of it, under it, or two steps under it
	//If so, then it checks if there exists a similar tile in appropriate neighbours that can be swapped to make the tiles matchable
	while (i <= BoardSize) {
		j = 1;
		while (j <= BoardSize) {
			if (Board[i][j].kind == Board[i + 1][j].kind) {
				if (i + 2 <= BoardSize) {
					if (Board[i + 2][j - 1].kind == Board[i][j].kind || Board[i + 2][j + 1].kind == Board[i][j].kind || Board[i + 3][j].kind == Board[i][j].kind) {
						matchFound = 1;
						break;
					}
				}
				if (i - 1 > 0) {
					if (Board[i - 1][j - 1].kind == Board[i][j].kind || Board[i - 1][j + 1].kind == Board[i][j].kind || Board[i - 2][j].kind == Board[i][j].kind) {
						matchFound = 1;
						break;
					}
				}
			}
			if (Board[i][j].kind == Board[i][j + 1].kind) {
				if (j + 2 <= BoardSize) {
					if (Board[i - 1][j + 2].kind == Board[i][j].kind || Board[i + 1][j + 2].kind == Board[i][j].kind || Board[i][j + 3].kind == Board[i][j].kind) {
						matchFound = 1;
						break;
					}
				}
				if (j - 1 > 0) {
					if (Board[i - 1][j - 1].kind == Board[i][j].kind || Board[i + 1][j - 1].kind == Board[i][j].kind || Board[i][j - 2].kind == Board[i][j].kind) {
						matchFound = 1;
						break;
					}
				}
			}
			if (i + 2 <= BoardSize) {
				if (Board[i][j].kind == Board[i + 2][j].kind) {
					if (Board[i + 1][j - 1].kind == Board[i][j].kind || Board[i + 1][j + 1].kind == Board[i][j].kind) {
						matchFound = 1;
						break;
					}
				}
			}
			if (j + 2 <= BoardSize) {
				if (Board[i][j].kind == Board[i][j + 2].kind) {
					if (Board[i + 1][j + 1].kind == Board[i][j].kind || Board[i - 1][j + 1].kind == Board[i][j].kind) {
						matchFound = 1;
						break;
					}
				}
			}
			j++;
		}
		if (matchFound)
			break;
		i++;
	}
	if(!matchFound)
		return false;

	//if hint is required, show the matchable tile by cutting down its opacity and making it jump
	if (hint) {
		Board[i][j].alpha = 150;
		Board[i][j].yCord -= 30;
	}
	return true;
}

//Checks if the mouse position is inside a rectangle shape of sfml. Used to see if mouse is clicked over a button
bool isInside(Window& window, RectangleShape& r1) {

	if (Mouse::getPosition(window).x >= r1.getPosition().x
		&& Mouse::getPosition(window).x <= r1.getPosition().x + r1.getSize().x
		&& Mouse::getPosition(window).y >= r1.getPosition().y
		&& Mouse::getPosition(window).y <= r1.getPosition().y + r1.getSize().y) {
		return true;
	}
	else
		return false;
}

void notification(RenderWindow& window, int lvl) {
	Font f1;
	f1.loadFromFile("bejeweled.ttf");

	string str = "Reached Level ";
	str += to_string(lvl);
	str+= "!!";
	
	float a = 100;

	Text notificationText;
	notificationText.setFont(f1);
	notificationText.setFillColor(Color(0, 255, 60, a));
	notificationText.setCharacterSize(40);
	notificationText.setString(str);
	notificationText.setStyle(Text::Bold);
	notificationText.setPosition(Vector2f(70, 530));


	Clock c;
	
	while (true) {
		Time t = c.getElapsedTime();
		if (t.asMilliseconds() > 1000) {
			break;
		}
		notificationText.setFillColor(Color(0, 255, 60, a));
		window.draw(notificationText);
		window.display();
		a++;
		

	}
}

int main()
{
	

	//Create Window
	RenderWindow window(VideoMode(1024, 640), "Bejeweled");
	window.setFramerateLimit(60);

	//Load textures
	Texture t1, t2, t3 ,t4, t5 ,t6, t7;
	if (!t1.loadFromFile("images/background.jpg")) {
		cout << "Could not load background.jpg" << endl;
	}
	if (!t2.loadFromFile("images/tiles.png")) {
		cout << "Could not load tiles.jpg" << endl;
	}
	if (!t3.loadFromFile("images/cursor.png")) {
		cout << "Could not load cursor.jpg" << endl;
	}
	if (!t4.loadFromFile("images/progressBar.png")) {
		cout << "Could not load progressBar.jpg" << endl;
	}
	if (!t5.loadFromFile("images/gameover.png")) {
		cout << "Could not load gameover.jpg" << endl;
	}
	if (!t6.loadFromFile("images/button.png")) {
		cout << "Could not load button.jpg" << endl;
	}
	if (!t7.loadFromFile("images/bejeweled.png")) {
		cout << "Could not load bejeweled.jpg" << endl;
	}

	//Create Sprites
	Sprite background(t1), gems(t2), cursor(t3), progressBarShape(t4), GameOver(t5), TitleScreen(t7);

	//Set main sprite positions
	GameOver.setPosition(Vector2f(440, 200));
	RectangleShape progressBar;
	progressBar.setFillColor(Color(0, 255, 60, 150));
	progressBar.setPosition(Vector2f(530, 530));
	progressBarShape.setPosition(Vector2f(520, 520));
	TitleScreen.setPosition(Vector2f(310, 150));
	progressBarShape.setPosition(Vector2f(520, 520));
	progressBar.setPosition(Vector2f(530, 530));

	//Load and Set Music
	Music music;
	if (!music.openFromFile("audio/soundtrack.wav")) {
		cout << "Could not load soundtrack.wav" << endl;
	}
	music.setLoop(true);
	music.play();
	music.setVolume(10);

	//Load Fonts
	Font f1;
	f1.loadFromFile("bejeweled.ttf");
	

	//Create All texts
	Text ScoreText;
	ScoreText.setFont(f1);
	ScoreText.setFillColor(Color(10, 80, 250, 255));
	ScoreText.setCharacterSize(40);
	ScoreText.setString("SCORE");
	ScoreText.setStyle(Text::Bold);
	ScoreText.setOutlineThickness(5);
	ScoreText.setOutlineColor(Color::Black);
	
	Text Score;
	Score.setFont(f1);
	Score.setFillColor(Color(0, 255, 60, 255));
	Score.setCharacterSize(40);
	Score.setStyle(Text::Bold);
	Score.setOutlineThickness(5);
	Score.setOutlineColor(Color::Black);
	
	Text GoalText;
	GoalText.setFont(f1);
	GoalText.setFillColor(Color(10, 80, 250, 255));
	GoalText.setCharacterSize(30);
	GoalText.setString("GOAL");
	GoalText.setStyle(Text::Bold);
	GoalText.setOutlineThickness(5);
	GoalText.setOutlineColor(Color::Black);
	
	Text Goal;
	Goal.setFont(f1);
	Goal.setFillColor(Color(0, 255, 60, 255));
	Goal.setCharacterSize(30);
	Goal.setStyle(Text::Bold);
	Goal.setOutlineThickness(5);
	Goal.setOutlineColor(Color::Black);

	Text LevelText;
	LevelText.setFont(f1);
	LevelText.setFillColor(Color(10, 80, 250, 255));
	LevelText.setCharacterSize(30);
	LevelText.setString("Level");
	LevelText.setStyle(Text::Bold);
	LevelText.setOutlineThickness(5);
	LevelText.setOutlineColor(Color::Black);

	Text Level;
	Level.setFont(f1);
	Level.setFillColor(Color(0, 255, 60, 255));
	Level.setCharacterSize(30);
	Level.setStyle(Text::Bold);
	Level.setOutlineThickness(5);
	Level.setOutlineColor(Color::Black);

	//Set position of All texts
	ScoreText.setPosition(Vector2f(100, 100));
	Score.setPosition(Vector2f(300, 100));
	GoalText.setPosition(Vector2f(540, 570));
	Goal.setPosition(Vector2f(640, 570));
	LevelText.setPosition(Vector2f(750, 570));
	Level.setPosition(Vector2f(850, 570));


	//Create All buttons and corresponding Text, and set positions
	RectangleShape Start;
	Start.setSize(Vector2f(250, 45));
	Start.setPosition(390, 440);
	Start.setTexture(&t6);
	Text StartText;
	StartText.setFont(f1);
	StartText.setFillColor(Color(240, 60, 240, 255));
	StartText.setCharacterSize(30);
	StartText.setString("START GAME");
	StartText.setStyle(Text::Bold);
	StartText.setOutlineThickness(2);
	StartText.setOutlineColor(Color::White);
	StartText.setPosition(430, 445);

	RectangleShape Hint;
	Hint.setSize(Vector2f(200, 45));
	Hint.setPosition(140, 240);
	Hint.setTexture(&t6);
	Text HintText;
	HintText.setFont(f1);
	HintText.setFillColor(Color(10, 80, 250, 255));
	HintText.setCharacterSize(30);
	HintText.setString("HINT");
	HintText.setStyle(Text::Bold);
	HintText.setOutlineThickness(1);
	HintText.setOutlineColor(Color::Black);
	HintText.setPosition(210, 245);

	RectangleShape Restart;
	Restart.setSize(Vector2f(200, 45));
	Restart.setPosition(30, 300);
	Restart.setTexture(&t6);
	Text RestartText;
	RestartText.setFont(f1);
	RestartText.setFillColor(Color(10, 80, 250, 255));
	RestartText.setCharacterSize(30);
	RestartText.setString("RESTART");
	RestartText.setStyle(Text::Bold);
	RestartText.setOutlineThickness(1);
	RestartText.setOutlineColor(Color::Black);
	RestartText.setPosition(80, 305);

	RectangleShape Classic;
	Classic.setSize(Vector2f(200, 45));
	Classic.setPosition(250, 300);
	Classic.setTexture(&t6);
	Text ClassicText;
	ClassicText.setFont(f1);
	ClassicText.setFillColor(Color(10, 80, 250, 255));
	ClassicText.setCharacterSize(30);
	ClassicText.setString("Classic");
	ClassicText.setStyle(Text::Bold);
	ClassicText.setOutlineThickness(1);
	ClassicText.setOutlineColor(Color::Black);
	ClassicText.setPosition(290, 305);
	

	RectangleShape Timed;
	Timed.setSize(Vector2f(200, 45));
	Timed.setPosition(30, 360);
	Timed.setTexture(&t6);
	Text TimedText;
	TimedText.setFont(f1);
	TimedText.setFillColor(Color(10, 80, 250, 255));
	TimedText.setCharacterSize(30);
	TimedText.setString("Timed");
	TimedText.setStyle(Text::Bold);
	TimedText.setOutlineThickness(1);
	TimedText.setOutlineColor(Color::Black);
	TimedText.setPosition(70, 365);

	RectangleShape Adrenaline;
	Adrenaline.setSize(Vector2f(200, 45));
	Adrenaline.setPosition(250, 360);
	Adrenaline.setTexture(&t6);
	Text AdrenalineText;
	AdrenalineText.setFont(f1);
	AdrenalineText.setFillColor(Color(10, 80, 250, 255));
	AdrenalineText.setCharacterSize(30);
	AdrenalineText.setString("Adrenaline");
	AdrenalineText.setStyle(Text::Bold);
	AdrenalineText.setOutlineThickness(1);
	AdrenalineText.setOutlineColor(Color::Black);
	AdrenalineText.setPosition(275, 365);
	
	RectangleShape Exit;
	Exit.setSize(Vector2f(200, 45));
	Exit.setPosition(140, 420);
	Exit.setTexture(&t6);
	Text ExitText;
	ExitText.setFont(f1);
	ExitText.setFillColor(Color(10, 80, 250, 255));
	ExitText.setCharacterSize(30);
	ExitText.setString("EXIT");
	ExitText.setStyle(Text::Bold);
	ExitText.setOutlineThickness(1);
	ExitText.setOutlineColor(Color::Black);
	ExitText.setPosition(210, 425);
	
	//seed random
	srand(time(0));
	//Initialize the Board
	int i = 1;
	int j = 1;
	while (i <= BoardSize) {
		j = 1;
		while (j <= BoardSize)
		{
			Board[i][j].kind = rand() % 9;
			Board[i][j].col = j;
			Board[i][j].row = i;
			Board[i][j].xCord = j * tileSize;
			Board[i][j].yCord = i * tileSize;
			j++;
		}
		i++;
	}

	//Storage Variables

	int x1, y1, x2, y2;				//storing cordinates
	int click = 0;					//count clicks
	Vector2i mousePos;				//relative mouse position

	//Flags
	bool isSwapped = false;
	bool isMoving = false;
	bool isSelected = false;

	bool gameOver = false;
	bool isRestarting = false;

	//Game State Variables
	int currentMode = 0;
	int score = 0;
	int tempScore = 0;
	int requiredScore = 50;
	int scoreIncrement = 50;
	int currentLevel = 1;
	int availableHints = 1;

	//Initialize clock
	Clock clock;

	
	


	//Game Loop
	while (window.isOpen())
	{	
		

		Event e;
		while (window.pollEvent(e))
		{
			if (e.type == Event::Closed)
				window.close();
			
			//if mouse on title screen then check if it presses start game
			else if (e.type == Event::MouseButtonPressed && !currentMode) {
				if (e.key.code == Mouse::Left)
				{
					if (isInside(window, Start)) {
						currentMode = 1;
					}
					
				}
			}
			//if in game state...
			else if (e.type == Event::MouseButtonPressed && currentMode)
			{
				if (e.key.code == Mouse::Left)
				{	
					//...then checks if mouse hits any menu button and respond accordingly 
					if (isInside(window, Hint)) {
						if (availableHints) {
							availableHints--;
							//In classic mode, reset progress after taking hint
							if (currentMode == 1) {
								tempScore = 0;
							}
							checkMatchable(1);
						}						
					}

					//Game Mode buttons reset initial conditions of the mode and clear the board

					if (isInside(window, Restart)) {
						clearBoard();
						isRestarting = true;
						score = 0;
						if (currentMode == 1) {
							requiredScore = 100;
							tempScore = 0;
						}
						if (currentMode == 2) {
							requiredScore = 100;
							tempScore = requiredScore / 2;
						}
						if (currentMode == 3) {
							requiredScore = 500;
							tempScore = requiredScore / 2;

						}
						currentLevel = 1;
						gameOver = 0;
						RestartText.setFillColor(Color(240, 60, 240, 255));
					}
					if (isInside(window, Classic)) {
						clearBoard();
						isRestarting = true;
						score = 0;
						tempScore = 0;
						requiredScore = 50;
						currentMode = 1;
						currentLevel = 1;
						gameOver = 0;
						ClassicText.setFillColor(Color(240, 60, 240, 255));
					}
					if (isInside(window, Timed)) {
						clearBoard();
						isRestarting = true;
						score = 0;
						currentMode = 2;
						currentLevel = 1;
						requiredScore = 100;
						tempScore = requiredScore / 2;
						gameOver = 0;
						TimedText.setFillColor(Color(240, 60, 240, 255));
					}
					if (isInside(window, Adrenaline)) {
						clearBoard();
						isRestarting = true;
						score = 0;
						currentMode = 3;
						currentLevel = 1;
						requiredScore = 500;
						tempScore = requiredScore / 2;
						gameOver = 0;
						AdrenalineText.setFillColor(Color(240, 60, 240, 255));
					}

					//Exit game if exit button pressed
					if (isInside(window, Exit)) {
						window.close();
					}

					if (!isSwapped && !isMoving) {
						click++;
					}

					//Map mouse position absolute to make it relative to that of board
					mousePos = Mouse::getPosition(window) - offset;

					//handle stretched screen
					mousePos = (Vector2i)window.mapPixelToCoords(mousePos);
				}
			} 
			
		}

		//Game State
		if (currentMode) {

			// If mouse clicked, check if it is inside Board. If so, then put cursor over tile
			if (click == 1 && mousePos.x > 0 && mousePos.y > 0)
			{

				x1 = mousePos.x / tileSize + 1;
				y1 = mousePos.y / tileSize + 1;

				if (x1 > 0 && x1 <= BoardSize && y1 > 0 && y1 <= BoardSize)
				{
					//set cursor
					cursor.setPosition(Board[y1][x1].xCord, Board[y1][x1].yCord);
					cursor.move(offset.x - tileSize, offset.y - tileSize);
					isSelected = true;
				}
				else
				{
					click = 0;
				}
				
			}

			// If clicked again, check if inside board and see if a neighbouring tile was clicked, if so swap them
			else if (click == 2 && mousePos.x > 0 && mousePos.y > 0)
			{
				x2 = mousePos.x / tileSize + 1;
				y2 = mousePos.y / tileSize + 1;
				if (x2 > 0 && x2 <= BoardSize && y2 > 0 && y2 <= BoardSize && abs(x2 - x1) + abs(y2 - y1) == 1) // 1 tile away
				{
					swap(Board[y1][x1], Board[y2][x2]);
					isSwapped = 1;
					
				}
				click = 0;
				isSelected = false;
			}
			//Reset after two clicks, or if pressed outside board
			else
			{
				click = 0;
			}

			//Check if any tiles are matched vertically or horizontally by checking the tiles ahead and behind them
			i = 1;
			j = 1;
			while (i <= BoardSize) {
				j = 1;
				while (j <= BoardSize)
				{
					if (Board[i][j].kind == Board[i + 1][j].kind) {
						if (Board[i][j].kind == Board[i - 1][j].kind) {
							int n = -1;
							while (n <= 1) {
								Board[i + n][j].isMatched++;
								n++;
							}
						}
					}

					if (Board[i][j].kind == Board[i][j + 1].kind) {
						if (Board[i][j].kind == Board[i][j - 1].kind) {
							int n = -1;
							while (n <= 1) {
								Board[i][j + n].isMatched++;
								n++;
							}
						}
					}

					j++;
				}
				i++;
			}

			i = 1;
			j = 1;
			//If the c and y cordinates of tiles are different than the default, move them one pixel at a time to correct position (Movement animation)
			isMoving = false;
			while (i <= BoardSize) {
				j = 1;
				while (j <= BoardSize)
				{
					tile& p = Board[i][j];
					int dx, dy;
					int n = 0;
					while (n < moveSpeed)   
					{
						dx = p.xCord - p.col * tileSize; 
						dy = p.yCord - p.row * tileSize; 
						if (dx) p.xCord -= dx / abs(dx); 
						if (dy) p.yCord -= dy / abs(dy);
						n++;
					}
					if (dx || dy) {
						isMoving = 1;
					}
					j++;
				}
				i++;
			}

			if (isMoving) {
				isRestarting = false;
			}
			//If a tile was matched then cut down its alpha to make a disappearing animation

			i = 1;
			j = 1;
			if (!isMoving) {
				while (i <= BoardSize) {
					j = 1;
					while (j <= BoardSize) {
						if (Board[i][j].isMatched) {
							if (Board[i][j].alpha > 10) {
								Board[i][j].alpha -= 10;
								isMoving = true;
							}
						}
						j++;
					}
					i++;
				}
			}

			//Check if any tile is matched and is remaining
			bool matched = 0;
			i = 1;
			j = 1;
			while (i <= BoardSize) {
				j = 1;
				while (j <= BoardSize) {
					matched += Board[i][j].isMatched;
					j++;
				}
				i++;
			}

			//If no tiles were matched, it means swap was useless so swap back in normal modes
			if (isSwapped && !isMoving)
			{
				if (!matched && currentMode != 3) {
					swap(Board[y1][x1], Board[y2][x2]);
				}
				isSwapped = 0;
			}

			//If no animationsa re running, then update the grid if possible
			if (!isMoving)
			{
				//brings the matched tiles which have now disappeared to the top (outside Board) hence bringing other tiles down
				i = BoardSize;
				j = 1;
				while (i > 0) {
					j = 1;
					while (j <= BoardSize) {
						if (Board[i][j].isMatched) {
							int n = i;
							while (n > 0) {
								if (!Board[n][j].isMatched) { 
									swap(Board[n][j], Board[i][j]);
									break;
								}
								n--;
							}
						}
						j++;
					}
					i--;
				}
				//creates new tiles at the top of the board and continues to grow them upwards, they will be brought back to correct position in moving animations part above
				j = 1;
				i = BoardSize;
				while (j <= BoardSize) {
					i = BoardSize;
					int n = 0;
					while (i > 0) {
						if (Board[i][j].isMatched)
						{
							Board[i][j].kind = rand() % 9;
							Board[i][j].yCord = -tileSize * n++;
							Board[i][j].isMatched = 0;
							Board[i][j].alpha = 255;
							if (!isRestarting) {
								score += 5 * currentLevel;	//update scores
								tempScore += 5 * currentLevel;
							}
						}
						i--;
					}
					j++;
				}
			}
			
			//while timed mode, decrease the tempscore to decrease the bar every second
			if (currentMode == 2 && !gameOver) {
				Time t = clock.getElapsedTime();
				if (t.asMilliseconds() > 1000) {
					clock.restart();
					tempScore-=currentLevel;
				}
			}

			//while adrenaline mode, decrease the tempscore to decrease the bar every 0.1 seconds
			if (currentMode == 3 && !gameOver) {
				Time t = clock.getElapsedTime();
				if (t.asMilliseconds() > 100) {
					clock.restart();
					tempScore -= currentLevel;
				}
			}

			//Check what percentage of the goal has been fulfilled and dill the progress bar to that percentage
			float percentageScore = tempScore * 100 / requiredScore;

			//Reset bar if goal was reached and advance the level and new goal
			if (percentageScore >= 100) {
				requiredScore += scoreIncrement;
				percentageScore = 0;
				tempScore = 0;

				//if timed reset to middle
				if (currentMode == 2) {
					tempScore = requiredScore / 2;
				}

				
				availableHints++;
				currentLevel++;
				
				notification(window, currentLevel);
			}

			float barLength = 330 * percentageScore / 100;
			

			progressBar.setSize(Vector2f(barLength, 16));
			Score.setString(to_string(score));
			Goal.setString(to_string(requiredScore));
			Level.setString(to_string(currentLevel));

			
			AdrenalineText.setFillColor(Color(10, 80, 250, 255));
			ClassicText.setFillColor(Color(10, 80, 250, 255));
			RestartText.setFillColor(Color(10, 80, 250, 255));
			TimedText.setFillColor(Color(10, 80, 250, 255));
			ExitText.setFillColor(Color(10, 80, 250, 255));

			// Highlight buttons if mouse hovers over them
			if (isInside(window, Restart)) {
				RestartText.setFillColor(Color(240, 60, 240, 255));
			}
			if (isInside(window, Classic)) {
				ClassicText.setFillColor(Color(240, 60, 240, 255));
			}
			if (isInside(window, Timed)) {
				TimedText.setFillColor(Color(240, 60, 240, 255));
			}
			if (isInside(window, Adrenaline)) {
				AdrenalineText.setFillColor(Color(240, 60, 240, 255));
			}
			if (isInside(window, Exit)) {
				ExitText.setFillColor(Color(240, 60, 240, 255));
			}

			if (availableHints) {
				HintText.setFillColor(Color(240, 60, 240, 255));
			}
			if (!availableHints) {
				HintText.setFillColor(Color( 10, 80, 250, 255));
			}


			//if There are no matches remaining after animations are done, then turn on gameover flag to present gameover screen and stop ingame state

			if (!isMoving && !checkMatchable() && currentMode!=3 || percentageScore < 0) {
				gameOver = true;
			}
			

		}
		
		//Draw the screen

		window.clear();
		window.draw(background);

		i = 1;
		j = 1;

		if (currentMode) {

			//Drawing the tiles
			i = 1;
			j = 1;
			while (i <= BoardSize) {
				j = 1;
				while (j <= BoardSize)
				{
					tile p = Board[i][j];
					gems.setTextureRect(IntRect(p.kind * 50, 0, 49, 49));
					gems.setColor(Color(255, 255, 255, p.alpha));
					gems.setPosition(p.xCord, p.yCord);
					gems.move(offset.x - tileSize, offset.y - tileSize);
					window.draw(gems);
					j++;
				}
				i++;
			}
			
			//Drawing the cursor
			if (isSelected)
			{
				window.draw(cursor);
			}

			//Drawing other elements like buttons, progress bar and text
			progressBarShape.setColor(Color(255, 255, 255, 255));
			window.draw(progressBarShape);
			window.draw(progressBar);
			window.draw(ScoreText);
			window.draw(Score);
			window.draw(GoalText);
			window.draw(Goal);
			window.draw(LevelText);
			window.draw(Level);
			window.draw(Restart);
			window.draw(RestartText);
			window.draw(Classic);
			window.draw(ClassicText);
			window.draw(Timed);
			window.draw(TimedText);
			window.draw(Adrenaline);
			window.draw(AdrenalineText);
			window.draw(Exit);
			window.draw(ExitText);
			window.draw(Hint);
			window.draw(HintText);


			//If game is over then display game over
			if (gameOver) {
				window.draw(GameOver);
			}
		}

		//If title screen then draw only title screen
		else {		
			window.draw(TitleScreen);
			window.draw(Start);
			window.draw(StartText);
		}

		window.display();
	}
	return 0;
}
