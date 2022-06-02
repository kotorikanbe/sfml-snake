# 《面向对象程序设计》大作业

## 1.编译环境说明
请保证c++标准在c++17及以上。
本程序使用了 SFML 库，配置完全参考[sfml tutorials](https://www.sfml-dev.org/tutorials/2.5/#getting-started)，简述如下：

- 从[下载链接](https://www.sfml-dev.org/download/sfml/2.5.1/)中下载对应你编译器的 SFML 压缩包。

  > 请注意，编译器版本必须与链接中提供版本号完全一致，否则请下载源码使用 cmake 自行创建适合你所用编译器版本的库（如 VS2022），自行配置可[参考连接](https://www.sfml-dev.org/tutorials/2.5/compile-with-cmake.php)。

- 下载源代码[压缩包](https://github.com/kotorikanbe/sfml-snake)并解压。
- 将压缩包中的**include**文件夹复制到解压的目录下，保证其和**makefile**文件处于同一目录。
- 将压缩包中的**lib**文件夹同样复制到解压目录下，保证其和**makefile**文件处于同一目录。
- 将**bin**文件夹中的**dll 文件**复制到解压目录下，请注意是**文件**而不是**文件夹**，或者将这些文件配置为系统环境变量。

  > 如果为自行编译的文件，**include**文件夹可在 SFML 源码中找到，静态库**lib**中的文件需要在运行 cmake 的 generate 后产生在 cmake 指定的**build**文件夹中，请找到对应的静态库文件和动态库文件按上述方法配置。

- 修改**Makefile**文件，示例如下：

```Makefile
INC_DIR =include #此为头文件相对路径，请填写你自己配置的路径。
LIB_DIR =lib #此为静态库相对路径，请填写你自己配置的路径
LIBS    =sfml-audio sfml-graphics sfml-main sfml-network sfml-system sfml-window #此为你要使用的静态库，请根据需求填写，如不清楚，请全部填写，细节可参考官网链接。
```

  > 请注意，如果不使用**Makefile**也可以直接在 IDE 中配置上述环境，这种情况下请让**Fonts,Music,Pictures,Sounds**文件夹以及**dll**文件与 IDE 生成的 exe 文件在同一目录下，并在 IDE 设置中设置好外部静态库，即可正常运行，请参考[SFML and Visual Studio](https://www.sfml-dev.org/tutorials/2.5/start-vc.php)。

- 测试
  创建新的文件，输入下列代码，并更改**Makefile**中的`SRCS`为你创建的文件的相对路径，在**命令提示符**或者**Powershell**中输入**make && main**进行测试。

<details>
<summary>参考代码</summary>

```c++
#include <SFML/Graphics.hpp>

int main()
{
    sf::RenderWindow window(sf::VideoMode(200, 200), "SFML works!");
    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        window.draw(shape);
        window.display();
    }

    return 0;
}
```

</code></pre>
</details>

- 如果正确配置，可得到以下结果。
  ![example](/Pictures/example.png)
- vscode 配置
  要正确的在 vscode 中对源代码进行编辑，你可能需要重新配置当前文件夹下的**json**文件。
  在**c_cpp_properties.json**文件中，你需要配置`includePath`和`path`中添加`"${workspaceFolder}/**"`来完成
  递归搜索，使智能引擎能正常使用。
  如果需要调试，你需要更改**tasks.json**文件的任务从默认的单独输入 g++以调试单个文件改为调用**make.exe**从而让编译器读取 Makefile 来编译。同时需保证**launch.json**中的`"program"`参数与**Makefile**中的`TARGET`参数两者名字保持一致，让 gdb 正确找到编译好的文件，代码参考如下。
<details>
<summary>参考代码</summary>

```json
#task.json
{
    "version": "2.0.0",
    "tasks":[
        {
            "label": "make",
            "type": "shell",
            "command": "mingw32-make.exe",
            "args": [],
            "presentation": {
                "echo": true,
                "reveal": "always",
                "focus": false,
                "panel": "shared",
                "showReuseMessage": true
            },
            "problemMatcher": [],
            "group": {
                "kind": "build",
                "isDefault": true
            }
        }
    ]
}
#launch.json
{
  "version": "0.2.0",
  "configurations": [
    {
      "name": "build with g++",
      "type": "cppdbg",
      "request": "launch",
      "targetArchitecture": "x86",
      "program": "${workspaceRoot}\\main.exe",
      "miDebuggerPath": "gdb",
      "args": [],
      "stopAtEntry": false,
      "cwd": "${workspaceRoot}",
      "externalConsole": false,
      "preLaunchTask": "make",
      "internalConsoleOptions": "neverOpen"
    }
    ]
}
```

</code></pre>
</details>

## 2.贪食蛇游戏设计说明
本代码基于[sfSnake](https://github.com/jhpy1024/sfSnake)进行改写并完善。
### 特性
1. 将原代码中用方向键进行操控的模式更改为鼠标操作，使蛇运动方向大幅增加，更加灵活。
2. 对原代码中的水果数量、颜色、计分方式进行优化，允许大量水果的出现（原代码有且仅有一个水果），增加游戏趣味性。
3. 对蛇的型态进行升级，全部运用**sf::sprite**进行绘制，并单独绘制蛇头，利于操控。
4. 增加网格线背景的选项，可以随意切换网格线颜色和背景颜色。
5. 提高刷新率，改为100hz，使在鼠标操控下蛇能更加灵活的移动。
### 2.1 鼠标操作的实现
原代码中使用的`enum class`在鼠标操作中不起作用，予以删除。在原代码中`move`、`grow`等操作均通过`enum class`判断方向来完成，我们首先在`sfSnake::Snake::handleInput`函数中对其进行更改，利用`sf::Mouse::isButtonPressed`来判断鼠标是否被点击，如被点击，运用`sf::Mouse::getPosition`函数得到鼠标点击的坐标，同时运用`nodes_[0].getPosition`找到的坐标，从而得到一个鼠标相对于蛇头的二维向量`sf::Vector2f`，为了消除鼠标距离的影响，将其单位化后记录为`sfSnake::Snake::direction_`，而不用`enum class`来记录`direction_`。同时由于采用sprite绘制会产生角度问题，
为了让蛇头始终指向鼠标点击方向，我们增加了`snakeNode`中的public变量arc来记录当前蛇节的角度，利用`std::atan2`函数求得对应向量的弧度值，且由于SFML只支持角度值，需要对其进行转换,我们同时给`Snake`中的也给予public变量`arc_`用于记录，然后在`move`、`grow`修改操作。代码参考如下。

<details>
<summary>参考代码</summary>

```c++
void Snake::handleInput(sf::RenderWindow &window, std::vector<Fruit> &fruits)
{
	checkSelfCollisions();
	checkFruitCollisions(fruits);
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		checkFruitCollisions(fruits);
		sf::Vector2f start = nodes_[0].getPosition();
		sf::Vector2i end = sf::Mouse::getPosition(window);
		sf::Vector2f vector(static_cast<float>(end.x) - start.x, static_cast<float>(end.y) - start.y);
		float tmp = std::atan2(vector.y, vector.x);
		vector.x = vector.x / sqrt(pow(vector.x, 2) + pow(vector.y, 2));
		vector.y = vector.y / sqrt(pow(vector.x, 2) + pow(vector.y, 2));
		direction_ = std::move(vector);
		arc_ = frad(tmp);
	}
	checkFruitCollisions(fruits);
	checkSelfCollisions();
//in grow
SnakeNode snakenode(sf::Vector2f(nodes_[nodes_.size() - 1].getPosition().x + direction_.x * SnakeNode::Width, nodes_[nodes_.size() - 1].getPosition().y + direction_.y * SnakeNode::Height), false, nodes_[nodes_.size() - 1].arc_);
//in move
nodes_[0].move((direction_.x * SnakeNode::Width) * 0.1, (direction_.y * SnakeNode::Height) * 0.1);
}
```

</code></pre>
</details>

### 2.2 多彩水果的实现
对于水果，总的来说有红、绿、蓝、褐、黑等5种颜色，但实际上为了色调丰富对于红、绿、蓝、褐、各有5种不同的水果，
即一共有21个颜色。SFML的颜色基于RGBA，故我们可以填入相应的RGBA值得到我们想要的颜色。
原代码中只有水果数为0时才进行增加，我们修改为随机增加，并控制随机概率来确保数量合适，同时控制5种颜色水果的比例为保持黑色和褐色水果所占比例为 25%，其他的占 75%。
此外在鼠标控制条件下可能会有按住鼠标水果不增加的bug，通过在`handleinput`下增加`generatefruit`函数可以解决。
同时，我们设定吃1个红色水果增加3个长度，蓝色为2个，绿色为1个，棕色黑色不增加，以增加游戏随机性，长度判断在`Grow`中实现。

<details>
<summary>参考代码</summary>

```c++
//in update and handleinput
static std::default_random_engine engine(time(NULL));
	static std::uniform_int_distribution<int> Distribution(0, 100000);
	snake_.checkFruitCollisions(fruit_);
    	if (Distribution(engine) % 200 == 0)
		generateFruit();
//random diatribute
void GameScreen::generateFruit()
{
	static std::default_random_engine engine(time(NULL));
	static std::uniform_int_distribution<int> xDistribution(0, Game::Width - SnakeNode::Width);
	static std::uniform_int_distribution<int> yDistribution(0, Game::Height - SnakeNode::Height);

	fruit_.push_back(Fruit(sf::Vector2f(xDistribution(engine), yDistribution(engine))));
}
//setcolor
sf::Color Fruit::getacolor(){
	static sf::Color brown1{140,81,25,255};
	static sf::Color brown2{139,105,20,255};
	static sf::Color brown3{139,69,19,255};
	static sf::Color brown4{165,42,42,255};
	static sf::Color brown5{205,149,12,255};
	static sf::Color blue1{152,245,255,255};
	static sf::Color blue2{0,0,255,255};
	static sf::Color blue3{135,206,250,255};
	static sf::Color blue4{65,105,255,255};
	static sf::Color blue5{100,149,237,255};
	static sf::Color red1{255,0,0,255};
	static sf::Color red2{255,69,0,255};
	static sf::Color red3{238,44,44,255};
	static sf::Color red4{205,85,85,255};
	static sf::Color red5{250,128,124,255};
	static sf::Color green1{0,255,0,255};
	static sf::Color green2{154,205,50,255};
	static sf::Color green3{0,255,127,255};
	static sf::Color green4{34,139,34,255};
	static sf::Color green5{32,178,170,255};
	static std::vector<sf::Color> Colors1{sf::Color::Black,brown1,brown2,brown3,brown4,brown5};
	static std::vector<sf::Color> Colors2{red1,red2,red3,red4,red5,blue1,blue2,blue3,blue4,blue5,green1,green2,green3,green4,green5};
	std::default_random_engine random(time(NULL));
    std::uniform_int_distribution<int> dis(0, 10000);
	int count=dis(random);
	if(count++%4==0){
		std::random_shuffle(Colors1.begin(),Colors1.end());
		return *Colors1.begin();
	}
	else{
		std::random_shuffle(Colors2.begin(),Colors2.end());
		return *Colors2.begin();
	}
}
sf::Color Fruit::getcolor()const{
	return shape_.getFillColor();
}
void Snake::grow(sf::Color color)
{
	static sf::Color brown1{140, 81, 25, 255};
	static sf::Color brown2{139, 105, 20, 255};
	static sf::Color brown3{139, 69, 19, 255};
	static sf::Color brown4{165, 42, 42, 255};
	static sf::Color brown5{205, 149, 12, 255};
	static sf::Color blue1{152, 245, 255, 255};
	static sf::Color blue2{0, 0, 255, 255};
	static sf::Color blue3{135, 206, 250, 255};
	static sf::Color blue4{65, 105, 255, 255};
	static sf::Color blue5{100, 149, 237, 255};
	static sf::Color red1{255, 0, 0, 255};
	static sf::Color red2{255, 69, 0, 255};
	static sf::Color red3{238, 44, 44, 255};
	static sf::Color red4{205, 85, 85, 255};
	static sf::Color red5{250, 128, 124, 255};
	static sf::Color green1{0, 255, 0, 255};
	static sf::Color green2{154, 205, 50, 255};
	static sf::Color green3{0, 255, 127, 255};
	static sf::Color green4{34, 139, 34, 255};
	static sf::Color green5{32, 178, 170, 255};
	static std::vector<sf::Color> blackandbrown{sf::Color::Black, brown1, brown2, brown3, brown4, brown5};
	static std::vector<sf::Color> red{red1, red2, red3, red4, red5};
	static std::vector<sf::Color> blue{blue1, blue2, blue3, blue4, blue5};
	static std::vector<sf::Color> green{green1, green2, green3, green4, green5};
	int count = 0;
	if (std::find(blackandbrown.begin(), blackandbrown.end(), color) != blackandbrown.end())
		count = 0;
	if (std::find(red.begin(), red.end(), color) != red.end())
		count = 3;
	if (std::find(blue.begin(), blue.end(), color) != blue.end())
		count = 2;
	if (std::find(green.begin(), green.end(), color) != green.end())
		count = 1;
	for (int i = 0; i < count;i++)
	{
		SnakeNode snakenode(sf::Vector2f(nodes_[nodes_.size() - 1].getPosition().x + direction_.x * SnakeNode::Width, nodes_[nodes_.size() - 1].getPosition().y + direction_.y * SnakeNode::Height), false, nodes_[nodes_.size() - 1].arc_);
		nodes_.push_back(snakenode);
	}
}

```

</code></pre>
</details>

### 2.3 **sprite**绘制与蛇节的实现
在原代码中，蛇节均为`sf::RectangleShape`进行绘制的，此方法绘制出来是一个方块，在只有方向键的情况下使用无大碍，但在鼠标操纵模式下，方形绘制在移动时会有割裂的感觉，特别是在刷新率低的情况下，为此，改用sprite进行绘制，将蛇头和蛇身绘制做到统一，以避免不同绘制方法产生的位移差，绘制所需的素材在**Pictures**文件夹中，且蛇头为网络截图，蛇身为自行绘图，均通过软件**Photoshop**完成。为了判断蛇头和蛇身，我们增加了`ishead`这一public变量进行判断。

<details>
<summary>参考代码</summary>

```c++
SnakeNode::SnakeNode(sf::Vector2f position, bool idt, float arc)
	:  ishead_(idt),arc_(arc),position_(position)
{
	if(ishead_){
		texture.loadFromFile("Pictures/head.png");
	}
	else{
		texture.loadFromFile("Pictures/body.png");
	}
	sprite.setTexture(texture);
	sprite.setPosition(position_);
	sprite.setScale(sf::Vector2f(0.1, 0.1));
	
}
```
</code></pre>
</details>

### 2.4 背景及网格线的实现
原代码中并未单独绘制背景，故其背景为命令行默认的黑色，我们在`Screen`绘制`snake`及`fruit`之前先对背景进行绘制，得到背景后再添加`snake`及`fruit`，而为了实现切换，我们首先修改操作逻辑，`GameOverScreen`按ENTER后跳回`MenuScreen`界面进行重新选择，对于`MenuScreen`我们也增加一系列按键选择，代码如下。

<details>
<summary>参考代码</summary>

```c++
//draw background and line
void GameScreen::render(sf::RenderWindow &window)
{
	sf::RectangleShape background(sf::Vector2f(Game::Width,Game::Height));
	background.setFillColor(backgroundcolor);
	window.draw(background);
	if(lines){
		for(int i=0;i<Game::Width;i+=10){
			sf::RectangleShape cline(sf::Vector2f(2.0,Game::Height));
			cline.setFillColor(linecolor);
			cline.setPosition(sf::Vector2f(static_cast<float>(i),0));
			window.draw(cline);
		}
		for(int i=0;i<Game::Height;i+=10){
			sf::RectangleShape cline(sf::Vector2f(Game::Width,2.0));
			cline.setFillColor(linecolor);
			cline.setPosition(sf::Vector2f(0,static_cast<float>(i)));
			window.draw(cline);
		}
	}
	snake_.render(window);

	for (auto fruit : fruit_)
		fruit.render(window);
}
//switch color in Menuscreen
void MenuScreen::handleInput(sf::RenderWindow &window)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		if (backgroundcolor == sf::Color::White)
		{

			backgroundcolor = sf::Color::Black;
			display1.setString("BACKGROUND:BLACK");
			display1.setFillColor(sf::Color::Black);
		}

		else if (backgroundcolor == sf::Color::Black)
		{
			backgroundcolor = Brown;
			display1.setString("BACKGROUND:BROWN");
			display1.setFillColor(Brown);
		}
		else if (backgroundcolor == Brown)
		{
			backgroundcolor = sf::Color::White;
			display1.setString("BACKGROUND:WHITE");
			display1.setFillColor(sf::Color::White);
		}
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)){
		Game::TimePerFrame=sf::seconds(1.0f/100.0f);
		Game::Screen = std::make_unique<GameScreen>(backgroundcolor, linescolor, lines);
		}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
		window.close();
	else if(sf::Keyboard::isKeyPressed(sf::Keyboard::L)){
		if(lines){
			lines=false;
			display2.setString("LINES:\t OFF");
			change.setString("Press [A] to change background color\n\nPress [L] to import lines\n\n");
		}
		else{
			lines=true;
			display2.setString("LINES:\t ON");
			change.setString("Press [A] to change background color\n\nPress [L] to import lines\n\nPress [s] to change lines color");
		}
	}
	else if(sf::Keyboard::isKeyPressed(sf::Keyboard::S)&& lines){
		if (linescolor == sf::Color::White)
		{

			linescolor = sf::Color::Black;
			display3.setString("LINES:BLACK");
			display3.setFillColor(sf::Color::Black);
		}

		else if (linescolor == sf::Color::Black)
		{
			linescolor = Brown;
			display3.setString("LINES:BROWN");
			display3.setFillColor(Brown);
		}
		else if (linescolor == Brown)
		{
			linescolor = sf::Color::White;
			display3.setString("LINES:WHITE");
			display3.setFillColor(sf::Color::White);
		}
	}
}
```
</code></pre>
</details>

### 2.5 高刷的实现
在原代码的像素风格的游戏下，刷新率只会对`Snake`的速度造成影响，而在鼠标操纵模式下，如果采用原代码的刷新率，可以看到肉眼可见的卡顿，因为鼠标操控使得`Snake`方向随时在改变，故在10hz下表现很差，同时由于判断角度的反正切函数并不是线性的，故横向移动时在10hz状态下很容易发生明显的抖动，这一切都要求我们提高屏幕的刷新率。
提高刷新率的方式很简单，我们直接把`TimePerFrame`这一变量修改为`sf::seconds(1.0f/100.0f)`即可，为此我们要解决很多高刷带来的兼容性bug。
1. 在MenuScreen界面不宜采用高刷，因为这会使变换按钮过快，按一次变换好几次的效果出现。
2. 如果同样采用单位长度的移动，那么与10hz相比运动快了10倍，但实际上原来的运动速度较为合适，所以我们每次运动幅度改为0.1个单位长度。
3. 原来的move函数实现在于下一帧蛇节的坐标等于上一个蛇节当前帧的坐标，这在移动单位长度上才能使用，移动0.1个单位长度使用会出现明显的bug（蛇会缩成一团），我们采用上下两帧向量差的0.1倍进行移动，一定程度上这会导致每一蛇节移动路径不完全一致，但这很符合实际蛇的实际运动情况。
4. 由于move函数的改动，原来的只判断蛇头出界即可的函数在新的move函数下有较大的bug，其余蛇节无法进行穿梭，为此我们采用坐标变换法，当蛇节穿墙后后续蛇节按照其穿墙后的坐标进行反变换的到不存在屏幕上的虚坐标用该坐标进行move操作，并且对每个蛇节单独进行穿墙操作，保证穿墙操作能成功运行。

<details>
<summary>参考代码</summary>

```c++
//move
void Snake::move()
{
	nodes_[0].arc_ = arc_;
	for (decltype(nodes_.size()) i = nodes_.size() - 1; i > 0; --i)
	{
		sf::Vector2f tmp1 = nodes_.at(i - 1).getPosition();
		sf::Vector2f tmp2 = nodes_[i].getPosition();
		sf::Vector2f tmp3 = tmp1 - tmp2;
		if (fabs(tmp3.x) > 50 || fabs(tmp3.y) > 50)
		{

			if (tmp3.x > 50)
			{
				tmp3.x = tmp1.x - Game::Width - tmp2.x;
			}
			if (tmp3.x < -50)
			{
				tmp3.x = tmp1.x + Game::Width - tmp2.x;
			}
			if (tmp3.y > 50)
			{
				tmp3.y = tmp1.y - Game::Height - tmp2.y;
			}
			if (tmp3.y < -50)
			{
				tmp3.y = tmp1.y + Game::Height - tmp2.y;
			}

			nodes_[i].setPosition(tmp2 + tmp3 * 0.1f);
			edgecrossing(nodes_[i]);
		}
		else
		{
			nodes_[i].setPosition(tmp2 + tmp3 * 0.1f);
		}
		nodes_[i].arc_ = nodes_[0].arc_;
	}
	nodes_[0].move((direction_.x * SnakeNode::Width) * 0.1, (direction_.y * SnakeNode::Height) * 0.1);
}
//edgecrossing
static void edgecrossing(SnakeNode &headNode)
{
	if (headNode.getPosition().x <= 0)
	{
		edge = headNode.getPosition();
		headNode.setPosition(Game::Width, headNode.getPosition().y);
	}
	else if (headNode.getPosition().x >= Game::Width)
	{
		edge = headNode.getPosition();
		headNode.setPosition(0, headNode.getPosition().y);
	}
	else if (headNode.getPosition().y <= 0)
	{
		edge = headNode.getPosition();
		headNode.setPosition(headNode.getPosition().x, Game::Height);
	}
	else if (headNode.getPosition().y >= Game::Height)
	{
		edge = headNode.getPosition();
		headNode.setPosition(headNode.getPosition().x, 0);
	}
}
```
</code></pre>
</details>

### 2.6 其他优化
1. 改了BGM没改音效。
2. 分辨率改为1080*720。
3. 更改了碰撞判定方式，从边缘覆盖改为距离。
4. 修复了原代码水果产生位置固定的bug。
5. 修复了原代码中声明与赋值顺序不一致的问题，修改了以及过时的函数，编译时不报错也没有警告。
### 2.7 当前可能存在的bug
1. 吃水果判定失效以及自杀判定失效的问题（经过反复调用这两个函数后失效率降低但依然存在，完整的修复估计需要利用多线程完美实现）
2. 有一定概率（蛇变得很长）时游戏结束后直接闪退，将`shared_ptr`改为`unique_ptr`后可能有所改善，不能完全排除。
3. 蛇头不跟手以及横向纵向移动时显然横向移动丝滑度不够，反三角函数问题，很难改善。