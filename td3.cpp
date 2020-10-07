#include <iostream>
#include "td3.hpp"
#include "support.hpp"
#include <stdlib.h>
#include <math.h>       // sin, cos

using namespace std;

using namespace support;

double* extend_array(double* array, int length, int new_size) {
  double *new_array = new double[new_size];
  for (int i = 0; i < length; i += 1){
      new_array[i] = array[i];
  }
  delete array;
  return new_array;
}

double* shrink_array(double* array, int length, int new_size) {
    double *new_array = new double[new_size];
    for (int i = 0; i < new_size; i += 1){
        new_array[i] = array[i];
    }
    delete array;
    return new_array;
}

double* append_to_array(double element,
                        double* array,
                        int &current_size,
                        int &max_size) {
  if (current_size == max_size) {
      double* new_array = extend_array(array, max_size, max_size + 5);
      max_size += 5;
      new_array[current_size] = element;
      current_size += 1;
      return new_array;
  }
  else {
    array[current_size] = element;
    current_size += 1;
    return array;
  }
}

double* remove_from_array(double* array,
                          int &current_size,
                          int &max_size) {
  if (current_size == 0) {
      return array;
  }
  if (max_size - current_size + 1 >= 5) {
      double* new_array = shrink_array(array, max_size, max_size - 5);
      current_size -= 1;
      max_size -= 5;
      return new_array;
  }
  else {
       current_size -= 1;
       return array;
    }

  return NULL; // YOU CAN CHANGE THIS
}

bool simulate_projectile(const double magnitude, const double angle,
                         const double simulation_interval,
                         double *targets, int &tot_targets,
                         int *obstacles, int tot_obstacles,
                         double* &telemetry,
                         int &telemetry_current_size,
                         int &telemetry_max_size) {
  // YOU CAN MODIFY THIS FUNCTION TO RECORD THE TELEMETRY

  bool hit_target, hit_obstacle;
  double v0_x, v0_y, x, y, t;
  double PI = 3.14159265;
  double g = 9.8;

  v0_x = magnitude * cos(angle * PI / 180);
  v0_y = magnitude * sin(angle * PI / 180);

  t = 0;
  x = 0;
  y = 0;

  hit_target = false;
  hit_obstacle = false;
  while (y >= 0 && (! hit_target) && (! hit_obstacle)) {
    double * target_coordinates = find_collision(x, y, targets, tot_targets);
    if (target_coordinates != NULL) {
      remove_target(targets, tot_targets, target_coordinates);
      hit_target = true;
    } else if (find_collision(x, y, obstacles, tot_obstacles) != NULL) {
      hit_obstacle = true;
    } else {
      telemetry = append_to_array(t, telemetry, telemetry_current_size, telemetry_max_size);
      telemetry = append_to_array(x, telemetry, telemetry_current_size, telemetry_max_size);
      telemetry = append_to_array(y, telemetry, telemetry_current_size, telemetry_max_size);
      t = t + simulation_interval;
      y = v0_y * t  - 0.5 * g * t * t;
      x = v0_x * t;
    }
  }
  if (hit_target == true || hit_obstacle == true){
    telemetry = append_to_array(t, telemetry, telemetry_current_size, telemetry_max_size);
    telemetry = append_to_array(x, telemetry, telemetry_current_size, telemetry_max_size);
    telemetry = append_to_array(y, telemetry, telemetry_current_size, telemetry_max_size);
  }
  return hit_target;
}

void sort(double *array, const int length) {
    for (int j = 0; j < length; j += 1){
        for (int i = 0; i < length - 3; i += 3){
            if (array[i] > array[i+3]){
                double tmp = array[i+3];
                array[i+3] = array[i];
                array[i] = tmp;
                tmp = array[i+4];
                array[i+4] = array[i+1];
                array[i+1] = tmp;
                tmp = array[i+5];
                array[i+5] = array[i+2];
                array[i+2] = tmp;
            }
        }
    }
}

void merge_telemetry(double **telemetries,
                     int tot_telemetries,
                     int *telemetries_sizes,
                     double* &telemetry,
                     int &telemetry_current_size,
                     int &telemetry_max_size) {
  telemetry_current_size = 0;
  telemetry_max_size = 0;
  for (int i = 0; i < tot_telemetries; i++){
      for (int j = 0; j < telemetries_sizes[i]; j++){
          telemetry = append_to_array(telemetries[i][j], telemetry, telemetry_current_size, telemetry_max_size);
      }
  }
  sort(telemetry, telemetry_current_size);
}
