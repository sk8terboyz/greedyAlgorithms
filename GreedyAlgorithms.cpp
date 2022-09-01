// This program goes over 5 basic greedy algorithms for bin packing
// By Shane Kennedy
#include <fstream>
#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
using namespace std;

//Prototypes
void randomNumberGeneration(ofstream& rawDataStream, vector<int>& randValues);
//method to place each number in the next available bin or create a new bin
void nextFit(ofstream& rawDataStream, vector<int>& randValues, vector<int>& nfBins, int binMax, int runCounter);
//method to place each number in the first bin available or creates a new bin
void firstFit(ofstream& rawDataStream, vector<int>& randValues, vector<int>& ffBins, int binMax, int runCounter);
//method to find which bin is the best fit for each number to get closest to the max, or creates a new bin
void bestFit(ofstream& rawDataStream, vector<int>& randValues, vector<int>& bfBins, int binMax, int runCounter);
//short method to sort the random numbers in descending order then calls firstFit
void ffDecreasing(ofstream& rawDataStream, vector<int>& randValues, vector<int>& ffdBins, int binMax, int runCounter);
//short method to sort the random numbers in descending order then calls bestFit
void bfDecreasing(ofstream& rawDataStream, vector<int>& randValues, vector<int>& bfdBins, int binMax, int runCounter);
//prints out the results of the data in accordance with the instructions of this assignment
void printResults(vector<int>& nfBins, vector<int>& ffBins, vector<int>& bfBins, vector<int>& ffdBins, vector<int>& bfdBins);

int main(int argc, char **argv)
{
  if(argc < 2)
  {
    cout<<"Insufficient arguments. Correct usage:"<< argv[0]<<" randomSeed"<<endl;
    return 1;
  }
  //initialize randomSeed variable
  int randomSeed;
  //set randomSeed equal to the random seed provided
  randomSeed = atoi(argv[1]);

  //if a randomSeed is provided, set srand to the seed
  if(randomSeed)
    srand(randomSeed);
  else
    srand(time(nullptr));

  //initialize randValues vector
  vector<int> randValues(100);
  int binMax = 100;

  //initialize vectors to keep track of number of bins in each experiment
  vector<int> nfBins(10);
  vector<int> ffBins(10);
  vector<int> bfBins(10);
  vector<int> ffdBins(10);
  vector<int> bfdBins(10);

  //counter for number of runs
  int runCounter = 0;

  ofstream rawDataStream("rawdata.txt");

  //run the experiment 10 times
  for(int i = 0; i < 10; i++)
  {
    rawDataStream << "*Experiment #" << i+1 << "*" << endl;
    randomNumberGeneration(rawDataStream, randValues);

    rawDataStream << "Next Fit:" << endl;
    nextFit(rawDataStream, randValues, nfBins, binMax, runCounter);

    rawDataStream << "\nFirst Fit:" << endl;
    firstFit(rawDataStream, randValues, ffBins, binMax, runCounter);

    rawDataStream << "\nBest Fit:" << endl;
    bestFit(rawDataStream, randValues, bfBins, binMax, runCounter);

    rawDataStream << "\nFirst Fit Decreasing:" << endl;
    ffDecreasing(rawDataStream, randValues, ffdBins, binMax, runCounter);

    rawDataStream << "\nBest Fit Decreasing:" << endl;
    bfDecreasing(rawDataStream, randValues, bfdBins, binMax, runCounter);
    runCounter++;
  }
  rawDataStream.close();

  printResults(nfBins, ffBins, bfBins, ffdBins, bfdBins);

}
//Random number generation
void randomNumberGeneration(ofstream& rawDataStream, vector<int>& randValues)
{
  int x;
  for(int i = 0; i < randValues.size(); i++)
  {
    x = 1 + rand()%100;
    rawDataStream << x << endl;
    randValues[i] = x;
  }
  rawDataStream<<endl;
}
//Next Fit bin packing algorithm
void nextFit(ofstream& rawDataStream, vector<int>& randValues, vector<int>& nfBins, int binMax, int runCounter)
{
  //initialize variable that tracks number of bins used
  int bins = 0;
  int binComparison = binMax;

  vector<vector<int>> binStorage;
  vector<int> binAdder;

  binAdder.push_back(0);
  binStorage.push_back(binAdder);

  //go through all random values
  for(int i = 0; i < randValues.size(); i++)
  {
    //if the value is within the parameters of each bin
    if(randValues[i] > binComparison)
    {
      //add a new bin
      bins++;
      //reset and decrease the size of the new bin
      binComparison = binMax - randValues[i];
      //push back value into new bin
      binAdder.clear();
      binAdder.push_back(randValues[i]);
      binStorage.push_back(binAdder);
      binStorage[bins].push_back(randValues[i]);
    }
    else
    {
      //add new value to that bin
      binStorage[bins].push_back(randValues[i]);
      //store the size of each bin when new values fit
      binStorage[bins][0] += randValues[i];
      //decrease size of the current bin while a new value can fit
      binComparison -= randValues[i];
    }

    //print the numbers to their corresponding bin number (1st part of next fit raw data)
    rawDataStream << randValues[i] << " " << bins << endl;
  }
  //empty line in the file
  rawDataStream<<endl;
  for(int i = 0; i < binStorage.size(); i++)
  {
    rawDataStream << i << " ";
    for(int j = 1; j < binStorage[i].size(); j++)
      rawDataStream << binStorage[i][j] << " ";
    rawDataStream << binStorage[i][0] << endl;
  }
  //set the number of bins used per run in the correct vector
  nfBins[runCounter] = bins;
}

//First Fit
void firstFit(ofstream& rawDataStream, vector<int>& randValues, vector<int>& ffBins, int binMax, int runCounter)
{
  //initialize bin counter
  int bins = 0;
  //initialize array to keep track of the space still in each bin
  int binSpace[randValues.size()];
  //initialize flag for the second for loop
  bool assigned = false;
  bool binCreated = true;
  //initialize vector to track space in each bin and store values
  vector<vector<int>> binStorage(1);
  vector<int> binAdder(1);
  //traverse through the random values
  for(int i = 0; i < randValues.size(); i++)
  {
    assigned = false;
    binCreated = true;

    int j;
    //check each bin from the beginning to see which has the space first
    for(j = 0; j < binStorage.size() && !assigned; j++)
    {
      if(binStorage[j][0] + randValues[i] <= 100)
      {
        binStorage[j].push_back(randValues[i]);
        binStorage[j][0] += randValues[i];
        assigned = true;
        binCreated = false;
        rawDataStream << randValues[i] << " " << j << endl;
      }
    }

    //if no bins have the space for the value
    if(binCreated)
    {
      //create a new bin and decrease the space within it by the value
      binAdder[0] = randValues[i];
      binStorage.push_back(binAdder);
      binStorage[bins].push_back(randValues[i]);
      rawDataStream << randValues[i] << " " << bins << endl;
      bins++;
    }
  }
  //empty line space
  rawDataStream << endl;
  //run through the data in the 2D vector and print it properly
  for(int i = 0; i < bins; i++)
  {
    rawDataStream << i << " ";
    for(int j = 1; j < binStorage[i].size(); j++)
    {
      rawDataStream << binStorage[i][j] << " ";
    }
    rawDataStream << binStorage[i][0] << endl;
  }
  //set the number of bins used per run in the correct vector
  ffBins[runCounter] = bins;
}

//Best Fit
void bestFit(ofstream& rawDataStream, vector<int>& randValues, vector<int>& bfBins, int binMax, int runCounter)
{
  //initialize bin counter
  int bins = 0;
  //initialize vectors to store storage space and values in each bin
  vector<vector<int>> binStorage(1);
  vector<int> binAdder(1);
  //go through the values 1 by 1
  for(int i = 0; i < randValues.size(); i++)
  {
    //initialize minimum space available in a bin
    int minSpaceAvailable = binMax + 1;
    //initialize best bin for a value to be placed in
    int bestBin = 0;

    for(int j = 0; j < bins; j++)
    {
      if(binStorage[j][0] >= randValues[i] && binStorage[j][0] - randValues[i] < minSpaceAvailable && binStorage[j][0] + randValues[i] < 100)
      {
        minSpaceAvailable = binStorage[j][0] - randValues[i];
        //set best bin for value to be inserted
        bestBin = j;
      }
    }
    //if the value does not fit in any bin
    if(minSpaceAvailable == binMax + 1)
    {
      //create new bin and store the value
      binAdder[0] = randValues[i];
      binStorage.push_back(binAdder);
      binStorage[bins].push_back(randValues[i]);
      binStorage[bins][0] = randValues[i];
      rawDataStream << randValues[i] << " " << bins << endl;
      bins++;
    }
    else
    {
      binStorage[bestBin].push_back(randValues[i]);
      binStorage[bestBin][0] += randValues[i];
      //resize minSpaceAvailable;
      minSpaceAvailable = minSpaceAvailable - randValues[i];
      rawDataStream << randValues[i] << " " << bestBin << endl;
    }
  }
  rawDataStream << endl;
  //run through the data in the 2D vector and print it properly
  for(int i = 0; i < bins; i++)
  {
    rawDataStream << i << " ";
    for(int j = 1; j < binStorage[i].size(); j++)
    {
      rawDataStream << binStorage[i][j] << " ";
    }
    rawDataStream << binStorage[i][0] << endl;
  }
  //set the number of bins used per run in the correct vector
  bfBins[runCounter] = bins;
}

//First Fit Decreasing
void ffDecreasing(ofstream& rawDataStream, vector<int>& randValues, vector<int>& ffdBins, int binMax, int runCounter)
{
  //sort the vector in decreasing order
  sort(randValues.begin(), randValues.end(), greater<int>());
  //call first fit
  firstFit(rawDataStream, randValues, ffdBins, binMax, runCounter);
}

//Best Fit Decreasing
void bfDecreasing(ofstream& rawDataStream, vector<int>& randValues, vector<int>& bfdBins, int binMax, int runCounter)
{
  //sort the vector in decreasing order
  sort(randValues.begin(), randValues.end(), greater<int>());
  //call best fit
  bestFit(rawDataStream, randValues, bfdBins, binMax, runCounter);
}

void printResults(vector<int>& nfBins, vector<int>& ffBins, vector<int>& bfBins, vector<int>& ffdBins, vector<int>& bfdBins)
{
  ofstream resultStream("results.txt");

  //print header
  resultStream << setw(5) << "Exp#" << setw(8) << "Next" << setw(8) << "First" << setw(8) << "Best"
      << setw(8) << "FFDec" << setw(8) << "BFDec"<< endl;
  //print dividng line
  for(int i = 0; i < 6; i++)
  {
    resultStream<<"------"<<setw(8);
  }
  resultStream<<"\n";

  //print all bins used in each experiment
  for(int i = 0; i < 10; i++)
  {
    resultStream<< setw(4) << i+1 << setw(8) << nfBins[i] << setw(8) << ffBins[i] << setw(8) << bfBins[i]
        << setw(8) << ffdBins[i] << setw(8) << bfdBins[i] << endl;
  }
  //print dividing line
  for(int i = 0; i < 6; i++)
  {
    resultStream<<"------"<<setw(8);
  }
  resultStream<<"\n";

  int sum = 0;
  vector<double> averages(5);

  for(int i = 0; i < nfBins.size(); i++)
  {
    sum += nfBins[i];
  }
  averages[0] = sum/10;
  sum = 0;

  for(int i = 0; i < ffBins.size(); i++)
  {
    sum += ffBins[i];
  }
  averages[1] = sum/10.0;
  sum = 0;

  for(int i = 0; i < bfBins.size(); i++)
  {
    sum += bfBins[i];
  }
  averages[2] = sum/10.0;
  sum = 0;

  for(int i = 0; i < ffdBins.size(); i++)
  {
    sum += ffdBins[i];
  }
  averages[3] = sum/10.0;
  sum = 0;

  for(int i = 0; i < bfdBins.size(); i++)
  {
    sum += bfdBins[i];
  }
  averages[4] = sum/10.0;
  sum = 0;
  //print the average bins used of each algorithm
  resultStream << setw(5) << "AVG:" << setw(8) << averages[0] << setw(8) << averages[1] << setw(8) << averages[2]
      << setw(8) << averages[3] << setw(8) << averages[4]<< endl;

  //print the dividing line
  for(int i = 0; i < 6; i++)
  {
    resultStream<<"------"<<setw(8);
  }
  resultStream<<"\n";

  vector<int> onlineBest = {0, 0, 0};
  for(int i = 0; i < 10; i++)
  {
    if(nfBins[i] <= ffBins[i] && nfBins[i] <= bfBins[i])
    {
      if(bfBins[i] == nfBins[i])
      {
        onlineBest[0]++;
        onlineBest[2]++;
      }
      else if(nfBins[i] == ffBins[i])
      {
        onlineBest[0]++;
        onlineBest[1]++;
      }
      else
        onlineBest[0]++;
    }
    else if(ffBins[i] <= bfBins[i] && ffBins[i] <= nfBins[i])
    {
      if(nfBins[i] == ffBins[i])
      {
        onlineBest[0]++;
        onlineBest[1]++;
      }
      else if(ffBins[i] == bfBins[i])
      {
        onlineBest[1]++;
        onlineBest[2]++;
      }
      else
        onlineBest[1]++;
    }
    else if(bfBins[i] <= ffBins[i] && bfBins[i] <= nfBins[i])
    {
      if(bfBins[i] == nfBins[i])
      {
        onlineBest[0]++;
        onlineBest[2]++;
      }
      else if(bfBins[i] == ffBins[i])
      {
        onlineBest[1]++;
        onlineBest[2]++;
      }
      else
        onlineBest[2]++;
    }
  }
  //print the number of times each online algorithm is the best
  resultStream<<setw(5) << "ONL:" << setw(7) << onlineBest[0] << setw(8) << onlineBest[1] << setw(8)
      << onlineBest[2] << setw(8) << "--" << setw(8) << "--" << endl;

  vector<int> allBest = {0, 0, 0, 0, 0};
  for(int i = 0; i < 10; i++)
  {
    if(nfBins[i] <= ffBins[i] && nfBins[i] <= bfBins[i] &&  nfBins[i] <= ffdBins[i] && nfBins[i] <= bfdBins[i])
    {
      if(nfBins[i] == ffBins[i])
      {
        allBest[0]++;
        allBest[1]++;
      }
      else if(nfBins[i] == bfBins[i])
      {
        allBest[0]++;
        allBest[2]++;
      }
      else if(nfBins[i] == ffdBins[i])
      {
        allBest[0]++;
        allBest[3]++;
      }
      else if(nfBins[i] == bfdBins[i])
      {
        allBest[0]++;
        allBest[4]++;
      }
      else
        allBest[0]++;
    }
    else if(ffBins[i] <= nfBins[i] && ffBins[i] <= bfBins[i] && ffBins[i] <= ffdBins[i] && ffBins[i] <= bfdBins[i])
    {
      if(ffBins[i] == nfBins[i])
      {
        allBest[0]++;
        allBest[1]++;
      }
      else if(ffBins[i] == bfBins[i])
      {
        allBest[1]++;
        allBest[2]++;
      }
      else if(ffBins[i] == ffdBins[i])
      {
        allBest[1]++;
        allBest[3]++;
      }
      else if(ffBins[i] == bfdBins[i])
      {
        allBest[1]++;
        allBest[4]++;
      }
      else
        allBest[1]++;
    }
    else if(bfBins[i] <= nfBins[i] && bfBins[i] <= ffBins[i] && bfBins[i] <= ffdBins[i] && bfBins[i] <= bfdBins[i])
    {
      if(bfBins[i] == nfBins[i])
      {
        allBest[0]++;
        allBest[2]++;
      }
      else if(bfBins[i] == ffBins[i])
      {
        allBest[1]++;
        allBest[2]++;
      }
      else if(bfBins[i] == ffdBins[i])
      {
        allBest[2]++;
        allBest[3]++;
      }
      else if(bfBins[i] == bfdBins[i])
      {
        allBest[2]++;
        allBest[4]++;
      }
      else
        allBest[2]++;
    }
    else if(ffdBins[i] <= nfBins[i] && ffdBins[i] <= ffBins[i] && ffdBins[i] <= bfBins[i] && ffdBins[i] <= bfdBins[i])
    {
      if(ffdBins[i] == nfBins[i])
      {
        allBest[0]++;
        allBest[3]++;
      }
      else if(ffdBins[i] == ffBins[i])
      {
        allBest[1]++;
        allBest[3]++;
      }
      else if(ffdBins[i] == bfBins[i])
      {
        allBest[2]++;
        allBest[3]++;
      }
      else if(ffdBins[i] == bfdBins[i])
      {
        allBest[3]++;
        allBest[4]++;
      }
      else
        allBest[3]++;
    }
    else if(bfdBins[i] <= nfBins[i] && bfdBins[i] <= ffBins[i] && bfdBins[i] <= bfBins[i] && bfdBins[i] <= ffdBins[i])
    {
      if(bfdBins[i] == nfBins[i])
      {
        allBest[0]++;
        allBest[4]++;
      }
      else if(bfdBins[i] == ffBins[i])
      {
        allBest[1]++;
        allBest[4]++;
      }
      else if(bfdBins[i] == bfBins[i])
      {
        allBest[2]++;
        allBest[4]++;
      }
      else if(bfdBins[i] == ffdBins[i])
      {
        allBest[3]++;
        allBest[4]++;
      }
      else
        allBest[4]++;
    }
  }
  //print the dividing line
  for(int i = 0; i < 6; i++)
  {
    resultStream<<"------"<<setw(8);
  }
  resultStream<<"\n";

  //print the best case of all algorithms
  resultStream <<setw(5) << "ALL:";
  resultStream << setw(7) << allBest[0];
  resultStream << setw(8) << allBest[1];
  resultStream << setw(8) << allBest[2];
  resultStream << setw(8) << allBest[3];
  resultStream << setw(8) << allBest[4] << endl;
  resultStream.close();
}
