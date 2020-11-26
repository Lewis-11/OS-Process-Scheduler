/**
* Final Parallel Programming Project
* @author Ignacio Talavera Cepeda (100383487)
* @author Luis Rodríguez Rubio (100383365)
* @author Eusebio Ricardo Carmona Milara (100383488)
* Date: 19/11/2019
* @version: 2.0
* Computer Architecture
* Computer Science and Engineering
* Universidad Carlos III de Madrid
*/

#include <iostream>
#include <sstream>
#include <stdio.h>
#include <vector>
#include <fstream>
#include <random>
#include <limits.h>
#include <stdlib.h>
#include <math.h>
#include <algorithm>
#include <iomanip>
#include <chrono>
#include <omp.h>

//Global variables, dimensions of space and constants
const double width = 200;
const double height = 200;
const double GRAVITY = 6.674e-5;
const double DELTA_T = 0.1;
const double MIN_DIS = 5.0;
const double mass = 1000;
const double sdm = 50;


class Asteroid {
  private:
    double mass;
    double x_pos;
    double y_pos;
    double x_vel;
    double y_vel;
    double x_force;
    double y_force;
    double x_acc;
    double y_acc;

    //neither acceleration nor force is a persistent data, it is only used to calculate the next position. We decided not to store it as a parameter.

  public:
    //Constructor
    Asteroid(double m, double x, double y){
      mass=m;
      x_pos=x;
      y_pos=y;
      x_vel=0;
      y_vel=0;
      x_force=0;
      y_force=0;
      x_acc=0;
      y_acc=0;

      //we are trusting the normal distribution to send correct coordinates
    }


    //Setters
    double setxPos(double x){
      if(x<0 || x>width){
        std::cerr <<"x coordinate is invalid, it must be between 0 and " << width << std::endl;
        return -1;
      }
      x_pos=x;
      return 0;
    }

    double setyPos(double y){
      if(y<0 || y>height){
        std::cerr <<"y coordinate is invalid, it must be between 0 and " << height << std::endl;
        return -1;
      }
      y_pos=y;
      return 0;
    }

    double setxVel(double xv){
      x_vel=xv;
      return 0;
    }

    double setyVel(double yv){
      y_vel=yv;
      return 0;
    }

    double setxForce(double xf){
      x_force=xf;
      return 0;
    }

    double setyForce(double yf){
      y_force=yf;
      return 0;
    }
    double setxAcc(double xa){
      x_acc=xa;
      return 0;
    }

    double setyAcc(double ya){
      y_acc=ya;
      return 0;
    }



    //Getters
    double getMass() const{
      return mass;
    }

    double getxPos() const{
      return x_pos;
    }

    double getyPos() const{
      return y_pos;
    }

    double getxVel() const{
      return x_vel;
    }

    double getyVel() const{
      return y_vel;
    }

    double getxForce() const{
      return x_force;
    }
    double getyForce() const{
      return y_force;
    }
    double getxAcc() const{
      return x_acc;
    }
    double getyAcc() const{
      return y_acc;
    }
};

class Planet{
  private:
    double mass;
    double x_pos;
    double y_pos;


  public:
    //Constructor
    Planet(double m, double x, double y){
      mass=m;
      x_pos=x;
      y_pos=y;
    }

     //Setters
    int setxPos(double x){
      if(x<0 || x>width){
        std::cerr <<"x coordinate is invalid, it must be between 0 and " << width << std::endl;
        return -1;
      }
      x_pos=x;
      return 0;
    }

    int setyPos(double y){
      if(y<0 || y>height){
        std::cerr <<"y coordinate is invalid, it must be between 0 and " << height << std::endl;
        return -1;
      }
      y_pos=y;
      return 0;
    }


    //Getters
    double getMass() const {
      return mass;
    }

    double getxPos() const {
      return x_pos;
    }

    double getyPos() const{
      return y_pos;
    }

    //It is likely that changing any of these will not be neccesary, but anyway, setters and getters may come in handy
};

/**
 * Dist function
 * @param a and @param b as objects of Asteroid class
 * @return the distance in between two objects
 * */

double dist (const Asteroid &a, const Asteroid &b){
  return sqrt(pow(a.getxPos()-b.getxPos(),2)+pow(a.getyPos()-b.getyPos(),2));
}

/**
 * Dist function
 * @param a and as object of Asteroid class
 * @param b as object of Planet class
 * @return the distance in between two objects
 * */

double dist (const Asteroid &a, const Planet &b){
  return sqrt(pow(a.getxPos()-b.getxPos(),2)+pow(a.getyPos()-b.getyPos(),2));
}

/**
 * Angle function
 * @param a and @param b as objects of Asteroid class
 * Calculates the slope in between the two objects as intermediate step, upper bounded to 1 and lower bounded to -1
 * We have to check the case that the two objects share the same x coordinate, in order to avoid a division
 * @return the angle of both objects
 * */


double slope ( const Asteroid &a, const Asteroid &b){
  double output = ( a.getyPos()-b.getyPos())/(a.getxPos()-b.getxPos());
  if(output>1){
    output = 1;
  }
  if(output<-1){
    output=-1;
  }
  return atan(output);
}

/**
 * Angle function
 * @param a and as object of Asteroid class
 * @param b as object of Planet class
 * Calculates the slope in between the two objects as intermediate step, upper bounded to 1 and lower bounded to -1
 * We have to check the case that the two objects share the same x coordinate, in order to avoid a division
 * @return the angle of both objects
 * */


double slope ( const Asteroid &a, const Planet &b){
  double output = ( a.getyPos() -b.getyPos())/(a.getxPos()-b.getxPos());
  if(output>1){
    output = 1;
  }

  if(output<-1){
    output=-1;
  }
  return atan(output);
}

/**
 * Attraction force function DIVIDED VERSION
 * @param a and @param b as objects of Asteroid class
 * @return the attraction force value in between objects a and b capped at 100
 * */

double aForceX(const Asteroid &a, const Asteroid &b){
  double angle = slope(a, b);
  double distance = dist(a,b);
  if (distance > MIN_DIS){
    double commonoperation = GRAVITY * a.getMass() * b.getMass() / pow(distance,2);
    return std::min(commonoperation*cos(angle),100.0);
  }
  else return 0;
}

double aForceY(const Asteroid &a, const Asteroid &b){
  double angle = slope(a, b);
  double distance = dist(a,b);
  if (distance > MIN_DIS){
    double commonoperation = GRAVITY * a.getMass() * b.getMass() / pow(distance,2);
    return std::min(commonoperation*sin(angle),100.0);
  }
  else return 0;
}


/**
 * Attraction force function DIVIDED VERSION
 * @param a  as object of Asteroid class
 * @param b as object of Planet Class
 * @return the attraction force value in between objects a and b capped at 100
 * */


double aForceX(const Asteroid &a, const Planet &b){
  double angle = slope(a, b);
  double commonoperation = GRAVITY * a.getMass() * b.getMass() / pow(dist(a,b),2);
  return std::min(commonoperation*cos(angle),100.0);
}

double aForceY(const Asteroid &a, const Planet &b){
  double angle = slope(a, b);
  double commonoperation = GRAVITY * a.getMass() * b.getMass() / pow(dist(a,b),2);
  return std::min(commonoperation*sin(angle),100.0);
}
/**
 * Acceleration force
 * @param a as vector with all the asteroids
 * @param n as the number of asteroids in the field
 * sets the correct acceleration in both axis for each of the asteroids
 * */

void refreshAcc (Asteroid* a) {
    (*a).setxAcc((*a).getxForce()/(*a).getMass());
    (*a).setyAcc((*a).getyForce()/(*a).getMass());
}

/**
 * Speed of each asteroid
 * @param a as vector with all the asteroids
 * @param n as the number of asteroids in the field
 * sets the correct speed in both axis for each of the asteroids
 * */

void refreshVel(Asteroid* a){
    (*a).setxVel((*a).getxVel() + (*a).getxAcc()*DELTA_T);
    (*a).setyVel((*a).getyVel() + (*a).getyAcc()*DELTA_T);
}

/**
 * Speed to refresh the position of each asteroid and calculate possible rebounds with borders
 * @param a as vector with all the asteroids
 * @param n as the number of asteroids in the field
 * sets the correct speed in both axis for each of the asteroids
 * */

void refreshPositions(Asteroid* a){
  double x, y;
    x = (*a).getxPos()+(*a).getxVel()*DELTA_T;
    y = (*a).getyPos()+(*a).getyVel()*DELTA_T;

    //Rebound checking

    if (x<=0){
      (*a).setxPos(MIN_DIS);
      (*a).setxVel((-1)*(*a).getxVel());
    }
    else if (x>=width){
      (*a).setxPos(width-MIN_DIS);
      (*a).setxVel((-1)*(*a).getxVel());
    }
    else{
      (*a).setxPos(x);
    }

    if (y<=0){
      (*a).setyPos(MIN_DIS);
      (*a).setyVel((-1)*(*a).getyVel());
    }
    else if (y>=width){
      (*a).setyPos(height-MIN_DIS);
      (*a).setyVel((-1)*(*a).getyVel());
    }
    else{
      (*a).setyPos(y);
    }
    //ifs instead of else-ifs because of the corner cases
}

void checkCollisions(Asteroid* a, Asteroid* b){
  if (dist((*a),(*b)) <= MIN_DIS){
    double velXA = (*a).getxVel();
    double velYA = (*a).getyVel();
    (*a).setxVel((*b).getxVel());
    (*a).setyVel((*b).getyVel());
    (*b).setxVel(velXA);
    (*b).setyVel(velYA);
  }
}

/**
* The inputs the program has to receive have to be:
* @param num_asteroids (int, >=0): indicates the number of asteroids that will
* be simulated
* @param num_iterations (int, >=0): indicates the number of iterations that will
* be simulated
* @param num_planets (int, >=0): indicates the number of plantes that will be
* included at the border of the space
* @param seed (int, >0): it is a number that serves as a base for the random
* generator functions
* @return -1 in case of error, 0 otherwise
*/

int main(int argc, char ** argv){

  auto t1 = std::chrono::high_resolution_clock::now();

  //Check correct number of parameters
  if( !(argc == 5)){
    std::cerr << "nasteroids-seq: Wrong arguments." << "\n" << "Correct Use:"
    << "\n" << "nasteroids-seq num_asteroids num_iterations num_planets seed"
    << "\n";
    return -1;
  }

  //Converts params to int
  int num_asteroids;
  int num_iterations;
  int num_planets;
  int seed;
  std::istringstream s1(argv[1]);
  std::istringstream s2(argv[2]);
  std::istringstream s3(argv[3]);
  std::istringstream s4(argv[4]);

  //check operation and endOfFile flag to detect errors or incorrect argument types
  if (!(s1 >> num_asteroids) || !(s2 >> num_iterations) || !(s3 >> num_planets) || !(s4 >> seed)
  ||!s1.eof() || !s2.eof() || !s3.eof() || !s4.eof()) {

    std::cerr << "nasteroids-seq: Wrong arguments." << "\n" << "Correct Use:"
    << "\n" << "nasteroids-seq num_asteroids num_iterations num_planets seed"
    << "\n";
    return -1;
  }

  //Random distributions initialization
  std::default_random_engine re{seed};
  std::uniform_real_distribution<double> ydist{0.0, std::nextafter(height, std::numeric_limits<double>::max())};
  std::uniform_real_distribution<double> xdist{0.0, std::nextafter(width, std::numeric_limits<double>::max())};
  std::normal_distribution<double> mdist{mass, sdm};

  //Print the initial parameters of the simulation into the corrresponding file
  std::ofstream initFile;
  initFile.open("init_conf.txt", std::ofstream::out);
  initFile << std::fixed;
  initFile << std::setprecision(3);

  for(int i = 1; i<argc; ++i){
    initFile << argv[i] << " ";
  }
  initFile << "\n";


  std::vector <Asteroid> asteroids;
  std::vector <Planet> planets;
  for (int i=0 ; i<num_asteroids; ++i){
    double x = xdist(re);
    double y = ydist(re);
    asteroids.push_back( Asteroid(mdist(re),x, y ) );
    //If the constructor is initialised directly with the random number generator,
    //the order of the coordinates is swapped

    initFile << asteroids[i].getxPos() << " " << asteroids[i].getyPos() << " " <<  asteroids[i].getMass() << "\n";

  }

//Planet initialization, placing it in each border
  for (int i=1; i<=num_planets; ++i){
    if (i%4==1){
      planets.push_back( Planet(mdist(re)*10, 0, ydist(re)) );
    }
    else if (i%4==2){
      planets.push_back( Planet(mdist(re)*10, xdist(re), 0) );
    }
    else if (i%4==3){
      planets.push_back( Planet(mdist(re)*10, width, ydist(re)) );
    }
    else {
      planets.push_back( Planet(mdist(re)*10, xdist(re), height) );
    }
    initFile << planets[i-1].getxPos() << " " << planets[i-1].getyPos() << " " <<  planets[i-1].getMass() << " ";
  }

  initFile.close();

  //opening solution file
  std::ofstream outFile;
  outFile.open("out_seq.txt", std::ofstream::out);
  outFile << std::fixed;
  outFile << std::setprecision(3);

  

  //Iterations
  int it = 1;
  while(it <= num_iterations){
      //This for loop resets the forces acting on the asteroids for each iteration

      for(int i = 0; i<num_asteroids; ++i){
        asteroids[i].setxForce(0);
        asteroids[i].setyForce(0);
        asteroids[i].setxAcc(0);
        asteroids[i].setyAcc(0);
        
      }
      // --- asteroids vs asteroids ---
      int size = (num_asteroids-1) * num_asteroids;
      int sizepl = num_asteroids*num_planets;
      std::vector<double> astforcesX(size);
      std::vector<double> astforcesY(size);
      std::vector<double> plforcesX(sizepl);
      std::vector<double> plforcesY(sizepl);

      for(int i = 0; i<num_asteroids; ++i){
        for(int j = i+1; j< num_asteroids; ++j){
          int index = num_asteroids-1;
          double iXForceJ = aForceX(asteroids[i], asteroids[j]);
          double iYForceJ = aForceY(asteroids[i], asteroids[j]);
          astforcesX[(index*i)+j-1] = iXForceJ;
          astforcesX[(index*j)+i] = -iXForceJ;

          astforcesY[(index*i)+j-1] = iYForceJ;
          astforcesY[(index*j)+i] = -iYForceJ;
        }
        // --- asteroids vs planets --- 
        for(int j = 0; j< num_planets; ++j){
          double iXForceJ = aForceX(asteroids[i], planets[j]);
          double iYForceJ = aForceY(asteroids[i], planets[j]);

          plforcesX[num_planets*i+j] = iXForceJ;
          plforcesY[num_planets*i+j] = iYForceJ;

        }
      }

      // These for loops cannot be parallelised, since the calculation of the forces has to be performed sequentially
      for(unsigned int i = 0; i<astforcesX.size(); ++i){
        asteroids[int(i / num_asteroids-1)].setxForce(asteroids[int(i / num_asteroids-1)].getxForce() + astforcesX[i]);
        asteroids[int(i / num_asteroids-1)].setyForce(asteroids[int(i / num_asteroids-1)].getyForce() + astforcesY[i]);
      }
      
      for(unsigned int i = 0; i<plforcesX.size(); ++i){
        asteroids[int(i/num_planets)].setxForce(asteroids[int(i/num_planets)].getxForce() + plforcesX[i]);
        asteroids[int(i/num_planets)].setyForce(asteroids[int(i/num_planets)].getyForce() + plforcesY[i]);
      }

      //refresh of Acc, vel and positions
      for (int i = 0; i<num_asteroids; ++i){
        refreshAcc(&asteroids[i]);
        refreshVel(&asteroids[i]);
        refreshPositions(&asteroids[i]);
      }

      //Colision checking
      
      for (int i = 0; i<num_asteroids; ++i){
        for (int j = i+1; j<num_asteroids; ++j){
          checkCollisions(&asteroids[i], &asteroids[j]);
        }
      }
      ++it;
  }

  //Final data of asteroids
  for(int i = 0; i< num_asteroids; ++i){
    double xpos = asteroids[i].getxPos();
    double ypos = asteroids[i].getyPos();
    double xvel = asteroids[i].getxVel();
    double yvel = asteroids[i].getyVel();
    double mass = asteroids[i].getMass();
    outFile << xpos << " " << ypos << " " << xvel << " " << yvel << " " << mass << "\n";

  }

  outFile.close();
  // stepFile.close();


  auto t2 = std::chrono::high_resolution_clock::now();
  auto dif = std::chrono::duration_cast<std::chrono::microseconds>(t2-t1);
  std::cout << dif.count() << std::endl;
  
  return 0;
}
