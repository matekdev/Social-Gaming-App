# Social Gaming App - Team Donair

Social Gaming App is a platform created to allow users to easily create games and play them with their friends. Users can create "games" by using our .json scripting language. These "games" can be created to either be multiplayer (ex. RockPaperScissors) or single player if you are feeling a little less social.

Here are a few screenshots of example usages...

A "GameSession" is created by providing the game file name found within `/src/Games`. This game file can be written using our "language" or by using included games we have written ourselves.

![img](https://i.gyazo.com/e2fc2e64cc734b2422ad78344b9a4151.png)

With the invitation code provided from using the `/createsession` command; you can now join the game session. A message is broadcasted to all other players within the game session that a user has joined. You can chat to other users within this game session while you wait for the game to start.

![img](https://i.gyazo.com/662e589822025a67eb421f20e479c960.png)

Lastly, you can start the game by using the `/startgame` command. This starts the game and now is able to be played by all users within the lobby. The example game below prints out variables stored within our custom .json scripting language to all users within the game.

![img](https://i.gyazo.com/62e57adc227d9f7db4029f48165ad295.png)

## How to build and use project

- ## Prerequisites

  - Boost 1.68
  - Ncurses
  - CMake
  - C++17

  

- ## Building

  - Download the repository, extract the folder to a location you have access to

  - Create a build folder

    - ```
      mkdir build
      cd build
      ```

  - Cmake to the location of the repository folder

    - ```
      sudo cmake PATH_TO_REPOSITORY_FOLDER
      
      Ex.
      sudo cmake /mnt/d/Dropbox/Dropbox/Courses/SPRING2020/CMPT373/social-gaming
      ```

  - Final step

    - ```
      sudo make GameServer
      ```

- ## Using

  - Go to the directory of where you built the project

    - ```
      ./bin/GameServer
      ```

  - Open the .html file and connect using "localhost:4000" or "localhost:4000"

## Project Structure

- ## src/

  - Contains source code and header files that are for internal use

- ## include/

  - Headers that are exposed publicly

- ## lib/

  - Third party libraries

- ## test/

  - Unit testing
  
## Usage
1. Create a .json "game" inside of /src/Games/...
2. Build and launch the server with the instructions above
3. Open the "socialapp.html" web client to connect into the server
4. Type in the IP address and click "connect"
5. You can type "/help" for the commands available
6. "/createsession Game.json" where "Game" is the name of your .json file
7. You should see an invite code be generated, type "/join invitecode"
8. Once in a gamesession, you can chat with other users, or start the game
9. Start the game by typing "/startgame invitecode"
10. The game will now run!

## Team Members
- Phan Bui
- Daniel Jui-Che Lin
- Hunsar Sekhon
- Joshua Deans
- Jimmy Zhong
- Matthew Zegar
- Lam Nguyen
- Yuheng Wu

## Build Requirements for Networking Libraries
- C++17 or newer
- Boost >= 1.66 and < 1.72
- CMake >= 3.12
- NCurses (only tested with 6.1)

## License

I was given permission by Professor Nick Sumner to be able to open source this project.

