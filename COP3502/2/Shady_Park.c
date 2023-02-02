#include <stdio.h>
#include <math.h>

#define AE 1e-8
#define pi 3.14159

//function to check that the value in the range of the tolerance
int absoluteError(double actual, double computed) {
    // Get the difference between the 2 values
    double diff = fabs(actual - computed);

    // Check if the values are within absolute error range
    if (diff < AE)
        return 1;
    // Values are not in the epsilon
    return 0;
}

//Function to calculate the percentage of the shadow area of the park:
//1st Case. When the sun angle is less than 90 degrees
double compute_shadowPercentage_0_90(int numTowers, int parkLength, double degrees, int x[], int h[])
{
    double sunArea = parkLength;    //store the sunned region
    int oldTower = 0;   //store the location of previous tower
    double oldShadow = 0.00000; //store the location of the farthest reaching shadow overall

    //loop to compute the location and shadows from each tower in order starting from West to East
    for (int t=0; t<numTowers; t++) {
        //compute the shadow range from a given tower
        double tower_shadow = fabs(h[t]/ tan((degrees * pi)/180));
        //store the location of the farthest reaching shadow from the given tower
        double newShadow = x[t] - tower_shadow;

        //if statements to compute the sunned regions
        //considering the cases when the given tower's shadow overlaps with the previous shadow
        //prevent double counting of the shadows

        //cases when the shadow falls beyond the westernmost location of the park
        if (newShadow <= 0) {
            newShadow = 0.00000;
            oldShadow = 0.00000;
        }
        //the case when the tower is at easternmost location
        if (x[t] == parkLength) {
            //shadow from the tower overshadow previous shadow
            if (newShadow <= oldShadow) {
                sunArea -= (parkLength - newShadow);
            }
            //shadow from the tower partially overlaps with precious shadow
            else if ((newShadow <= oldTower) && (newShadow > oldShadow)) {
                sunArea -= (parkLength - oldShadow);
            }
            //shadow does not overlap
            else {
                sunArea -= ((oldTower - oldShadow) + (parkLength - newShadow));
            }
        //for other towers to check shadow overlapping
        } else {
            //when the new shadow overshadow the old one
            if (newShadow <= oldShadow) {
                oldShadow = newShadow;  //new shadow is the farthest reaching shadow
            }
            //when new shadow does not overlap
            if (newShadow > oldTower) {
                sunArea -= (oldTower - oldShadow);
                oldShadow = newShadow;  //new shadow becomes the farthest reaching shadow
            } //if none of the if statements is true, the old shadow remains the farthest reaching shadow
        }

        oldTower = x[t]; //the tower in the current cycle becomes old in the next cycle

    //compute the shaded range percentage and return the value
    }
    double shadowed_percentage = ((parkLength - sunArea)/parkLength) * 100;
    return shadowed_percentage;
}

//2nd Case. When the sun angle is more than 90 degrees
double compute_shadowPercentage_90_180(int numTowers, int parkLength, double degrees, int x[], int h[])
{
    double sunArea = parkLength;    //store the sunned region
    int oldTower = parkLength;   //store the location of previous tower
    double oldShadow = parkLength; //store the location of the farthest reaching shadow overall

    //loop to compute the location and shadows from each tower in order starting from East to West
    for (int t=numTowers-1; t>=0; t--) {
        //compute the shadow range from a given tower
        double tower_shadow = fabs(h[t]/ tan(((180 - degrees) * pi)/180));
        //store the location of the farthest reaching shadow from the given tower
        double newShadow = x[t] + tower_shadow;

        //if statements to compute the sunned regions
        //considering the cases when the given tower's shadow overlaps with the previous shadow
        //prevent double counting of the shadows

        //cases when the shadow falls beyond the easternmost location of the park
        if (newShadow >= parkLength) {
            newShadow = parkLength;
            oldShadow = parkLength;
        }

        //the case when the tower is at westernmost location
        if (x[t] == 0) {
            //shadow from the tower overshadow previous shadow
            if (newShadow >= oldShadow) {
                sunArea -= (newShadow);
            }
                //shadow from the tower partially overlaps with precious shadow
            else if ((newShadow >= oldTower) && (newShadow < oldShadow)) {
                sunArea -= (oldShadow);
            }
                //shadow does not overlap
            else {
                sunArea -= ((oldShadow - oldTower) + (newShadow));
            }
            //for other towers to check shadow overlapping
        } else {
            //when the new shadow overshadow the old one
            if (newShadow >= oldShadow) {
                oldShadow = newShadow;  //new shadow is the farthest reaching shadow
            }
            //when new shadow does not overlap
            if (newShadow < oldTower) {
                sunArea -= (oldShadow - oldTower);
                oldShadow = newShadow;  //new shadow becomes the farthest reaching shadow
            } //if none of the if statements is true, the old shadow remains the farthest reaching shadow
        }
        oldTower = x[t]; //the tower in the current cycle becomes old in the next cycle

        //compute the shaded range percentage and return the value
    }

    double shadowed_percentage = ((parkLength - sunArea)/parkLength) * 100;
    return shadowed_percentage;
}


int main() {
    //Read in the information about the number of towers, length of park, and the percentage of sade needed in the park.
    int N_towers, L_of_park, P_shaded;
    scanf("%d", &N_towers);
    scanf("%d", &L_of_park);
    scanf("%d", &P_shaded);

    // Read in the height and location of each tower
    int x_newTower[N_towers], h_of_tower[N_towers];
    for (int n = 0; n < N_towers; n++) {
        scanf("%d", &x_newTower[n]);
        scanf("%d", &h_of_tower[n]);
    }

    //Using binary search to find the angle of the sun at which the shaded range will be true for the declared shade percentage.
    //Case 1. The sun is on the east (0-90 degrees)
    double lo1=0;
    double hi1=90;
    double angle1;
    double result1 = 1000;

    //binary search that compares the desired percentage and computed percentage to decide if the next angle should be higher or lower
    while (absoluteError(P_shaded, result1) != 1) {
        double mid1 = (lo1 + hi1) / 2;
        angle1 = mid1;

        result1 = compute_shadowPercentage_0_90(N_towers, L_of_park, angle1, x_newTower, h_of_tower);
        if (result1 < P_shaded){
            hi1 = mid1;
        } else {
            lo1 = mid1;
        }
    }
    double ans1 = lo1;

    //Case 2. The sun is on the west (90-180 degrees)
    double lo2=90;
    double hi2=180;
    double angle2;
    double result2 = 1000;

    //binary search that compares the desired percentage and computed percentage to decide if the next angle should be higher or lower
    while (absoluteError(P_shaded, result2) != 1) {
        double mid2 = (lo2 + hi2) / 2;
        angle2 = mid2;

        result2 = compute_shadowPercentage_90_180(N_towers, L_of_park, angle2, x_newTower, h_of_tower);
        if (result2 > P_shaded){
            hi2 = mid2;
        } else {
            lo2 = mid2;
        }
    }
    double ans2 = lo2;

    //Computed answer output
    printf("%.5lf %.5lf", ans1, ans2);

    return 0;
}
