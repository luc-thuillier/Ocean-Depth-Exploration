#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

using namespace std;


// Function to read the depth matrix from a file and store it in a 2D vector.
vector<vector<float>> readDepthMatrix(const string &fileName, int rows, int cols) { // Create a 2D vector with dimensions row x cols.
    vector<vector<float>> depths(rows, vector<float>(cols));
    ifstream inFile(fileName); // Opens specified file for reading.
    string line;


    if (!inFile.is_open()) {
        cerr << "Error opening file titled: " << fileName << endl;
        return {};
    }


    for (int i = 0; i < rows && getline(inFile, line); ++i) { // Reads file line by line and fills the vector.
        stringstream ss(line);
        for (int j = 0; j < cols && ss >> depths[i][j]; ++j) {
            if (ss.peek() == '\t') ss.ignore();
        }
    }


    inFile.close();
    return depths;
}


// Function to find and report the coordinates and depth of the deepest point in the depth matrix.
void findDeepestPoint(const vector<vector<float>> &depths, ostream &out) {
    float maxDepth = 0;
    int maxRow = 0, maxCol = 0;


    for (int i = 0; i < depths.size(); ++i) { // Iterates through each depth value and updates the maximum depth and its coordinates once a deeper point is found.
        for (int j = 0; j < depths[i].size(); ++j) {
            if (depths[i][j] > maxDepth) {
                maxDepth = depths[i][j];
                maxCol = i;
                maxRow = j;
            }
        }
    }


    out << "The deepest point is " << maxDepth << " meters, occurring at coordinates: (" << maxRow+1 << "," << maxCol+1 << ")" << endl;
}


// Function to calculate the deepest 2x2 area's average depth and coordinates.
void findDeepest2x2Area(const vector<vector<float>> &depths, ostream &out) {
    float maxAverageDepth = 0;
    int maxRow = 0, maxCol = 0;


    for (size_t i = 0; i < depths.size() - 1; ++i) { // Iterates through 2x2 areas within the matrix, and calculates the average depth, updates maximum average depth once a deeper average is found.
        for (size_t j = 0; j < depths[i].size() - 1; ++j) {
            float averageDepth = (depths[i][j] + depths[i][j+1] + depths[i+1][j] + depths[i+1][j+1]) / 4;
            if (averageDepth > maxAverageDepth) {
                maxAverageDepth = averageDepth;
                maxCol = i;
                maxRow = j;
            }
        }
    }


    out << "The deepest 2x2 area average depth is " << maxAverageDepth << " meters, occurring at coordinates: (" << maxRow+1 << "," << maxCol+1 << ")" << endl;
}


// Function to calculate the total volume represented by the depth matrix.
float total_volume(const vector<vector<float>> &depths) {
    float volume = 0;
    for (const auto &row : depths) {
        for (auto depth : row) {
            volume += depth;
        }
    }
    return volume;
}


int main() {
    cout<<"*********************************************************************************"<<endl; // Welcome the user.
    cout <<"Welcome to CPHW3. This is a Sea Floor Analysis program written by Luc Thuillier."<<endl;
    cout<<"*********************************************************************************"<<endl;
    cout<<endl;
    do {
        string fileName, outputFileName;
        int rows, cols;

        cout <<"Please enter the data file name (depth1.txt or depth2.txt): "; // Ask the user for necessary inputs.
        cin >> fileName;
        cout<<endl;
        cout <<"Please enter the number of rows and columns in the file (e.g., 6 6 for depth1.txt): ";
        cin >> rows >> cols;
        cout<<endl;


        auto depths = readDepthMatrix(fileName, rows, cols);
        if (depths.empty()) {
            cerr <<"Error: failed to read depths from the file."<< endl;
            return 1;
        }


        cout <<"Please enter the desired output file name: "; // Ask the user for a name for the output/txt file.
        cin >> outputFileName;
        cout<<endl;
        ofstream outFile(outputFileName);


        if (!outFile.is_open()) {
            cerr <<"Error opening output file titled: "<< outputFileName << endl;
            return 1;
        }


        findDeepestPoint(depths, cout); // Part A.
        findDeepestPoint(depths, outFile);
        findDeepest2x2Area(depths, cout); // Part B.
        findDeepest2x2Area(depths, outFile);
        float volume = total_volume(depths); // Part C.
        cout <<"The total volume of the sea floor section is "<< volume <<" cubic meters."<< endl;
        outFile <<"The total volume of the sea floor section is "<< volume <<" cubic meters."<< endl;
        cout<<endl;


        outFile.close(); // Close the file.


        // Ask the user if they wish to analyze another file (Y/N).
        char continueAnalysis;
        cout <<"The program is finished."<<endl;
        cout <<"Would you like to analyze another file? (Y/N): ";
        cin >> continueAnalysis;
        cout<<endl;
        if (toupper(continueAnalysis) != 'Y') {
            break; // Exit the loop and program if the user inputs "N".
        }
    } while (true);


    return 0;
}
