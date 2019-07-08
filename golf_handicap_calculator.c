#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "methods.h"

#define NUM_SCORES (5)

int main() {
  char db[5] = "\\\\";
  printf("\n\t\tWelcome to the Golf Handicap Calculator (C)\n");
  printf("\n");
  printf("\t\t           #                  # \n");
  printf("\t\t            #       GHC      # \n");
  printf("\t\t             #              # \n");
  printf("\t\t              #            # \n");
  printf("\t\t               %s        // \n", db);
  printf("\t\t                %s      // \n", db);
  printf("\t\t                 %s    // \n", db);
  printf("\t\t                  %s  // \n", db);
  printf("\t\t                   %s// \n", db);
  printf("\t\t                    //  \n");
  printf("\t\t                   //%s   \n", db);
  printf("\t\t                  //  %s    \n", db);
  printf("\t\t                 //    %s     \n", db);
  printf("\t\t                //      %s      \n", db);
  printf("\t\t               //        %s       \n", db);
  printf("\t\t          (###//    (*)   %s###)    \n", db);
  printf("\n");



  int option = 0;
  while (1) {
    while (1) {
      printf("Select An Option:\n");
      printf("1) Record Score and Add Differential to List\n");
      printf("2) Calculate Handicap\n");
      printf("3) Exit\n");
      printf("\nEnter: ");

      scanf("%d", &option);
      if (option == 1 || option == 2) {
        break;
      }
      if (option == 3) {
        return 0;
      }
    }

    if (option == 1) {
      printf("Select The Number of Holes Played:\n");
      int holes_played = 0;
      while (1) {
        printf("1) 9 Holes\n");
        printf("2) 18 Holes\n");
        printf("3) Exit\n");

        printf("\nEnter: ");
        scanf("%d", &holes_played);
        if (holes_played == 1 || holes_played == 2) {
          break;
        }
        if (holes_played == 3) {
          return 0;
        }
        printf("Handicap Calculator: Invalid Input - %d\n", holes_played);
      }

      printf("\nWhat Is The Course Rating Of the Course You Played?\n");
      printf("Rating: ");
      float rating = 0.0;
      scanf("%f", &rating);

      printf("\nWhat Is The Slope Of the Course You Played?\n");
      printf("Slope: ");
      float slope = 0.0;
      scanf("%f", &slope);

      printf("\nWhat Was Your Score Today?\n");
      printf("Score: ");
      float score = 0.0;
      scanf("%f", &score);

      if (holes_played == 1) {
        float nine_differential = calculate_differential_nine(rating, slope, score);
        record_differential_nine("nine_hole_differentials", nine_differential);
        printf("Differential (%f) has been recorded.\n", nine_differential);

      } else {
        float eighteen_differential = calculate_differential_eighteen(rating, slope, score);
        record_differential_eighteen("eighteen_hole_differentials", eighteen_differential);
        printf("Differential (%f) has been recorded.\n", eighteen_differential);
      }

    }
    else {
      // File Reading, Saving/Writing Score into file, Checking for 5 scores and discerning
      // which 5 scores are the best.
      printf("Please Select Your Preferred Handicap: \n");
      int handicap_option = 0;
      while (1) {
        printf("1) 9-Hole Handicap\n");
        printf("2) 18-Hole Handicap\n");
        printf("3) Exit\n");
        printf("Enter: ");
        scanf("%d", &handicap_option);

        if (handicap_option == 1 || handicap_option == 2) {
          break;
        }
        if (handicap_option == 3) {
          return 0;
        }
      }

      ////////////////////////////////////////////////////
      if (handicap_option == 1) {
        float nine_index = fetch_nine_handicap_index("nine_hole_differentials");
        printf("\n9-Hole Handicap: %.2f\n\n", nine_index);
        if (nine_index == 0.0) {
          printf("You do not have enough scores to calculate and accurate 9-hole handicap.\n");
        }

      } else {
        float eighteen_index = fetch_eighteen_handicap_index("eighteen_hole_differentials");
        printf("\n18-Hole Handicap: %.2f\n\n", eighteen_index);
        if (eighteen_index == 0.0) {
          printf("You do not have enough scores to calculate and accurate 18-hole handicap.\n");
        }
      }


    }
  }

  return 0;
} // Main()

float calculate_differential_nine(float rating, float slope, float score) {
  float adjusted_rating = rating / 2;
  return (score - adjusted_rating) * (113 / slope);
} // calculate_differential_nine()

float calculate_differential_eighteen(float rating, float slope, float score) {
  return (score - rating) * (113 / slope);
} // calculate_differential_eighteen()

void record_differential_nine(char* filename, float diff) {
  FILE* write_pointer = fopen(filename, "a");
  if (write_pointer == NULL) {
    printf("Differential could not be recorded. (Line 150)\n");
    return;
  }
  fprintf(write_pointer, "%f\n", diff);

  fclose(write_pointer);
  write_pointer = NULL;
} // record_differential_nine

void record_differential_eighteen(char* filename, float diff) {
  FILE* write_pointer = fopen(filename, "a");
  if (write_pointer == NULL) {
    printf("Differential could not be recorded. (Line 162)\n");
    return;
  }
  fprintf(write_pointer, "%f\n", diff);

  fclose(write_pointer);
  write_pointer = NULL;
} // record_differential_eighteen

float fetch_nine_handicap_index(char* filename) {
  FILE* read_pointer = fopen(filename, "r");
  if (read_pointer == NULL) {
    printf("Differential could not be fetched. (Line 174)\n");
  }

  int line_count = 0;
  float diff = 0.0;
  int scan_ret = 0;

  while (1) {
    scan_ret = fscanf(read_pointer, "%f", &diff);
    if (scan_ret == EOF) {
      break;
    }
    line_count++;
  }

  if (line_count < 5) {
    fclose(read_pointer);
    read_pointer = NULL;
    return 0.0;
  }

  fseek(read_pointer, 0, SEEK_SET);
  float arr[line_count];

  int i = 0;
  while (1) {
    scan_ret = fscanf(read_pointer, "%f", &diff);
    if (scan_ret == EOF) {
      break;
    }
    arr[i] = diff;
    i++;
  }

  float* sorted_arr = malloc(line_count * sizeof(float));
  sorted_arr = array_sort(arr, line_count);

  float arr_sum = 0.0;
  for (int i = 0; i < NUM_SCORES; i++) {
    arr_sum += sorted_arr[i];
  }

  float index = arr_sum / NUM_SCORES;

  fclose(read_pointer);
  read_pointer = NULL;

  return index;
} // fetch_nine_handicap_index

float fetch_eighteen_handicap_index(char* filename) {
  FILE* read_pointer = fopen(filename, "r");
  if (read_pointer == NULL) {
    printf("Differential could not be fetched. (Line 227)\n");
  }

  int line_count = 0;
  float diff = 0.0;
  int scan_ret = 0;

  while (1) {
    scan_ret = fscanf(read_pointer, "%f", &diff);
    if (scan_ret == EOF) {
      break;
    }
    line_count++;
  }
  if (line_count < NUM_SCORES) {
    fclose(read_pointer);
    read_pointer = NULL;
    return 0.0;
  }

  fseek(read_pointer, 0, SEEK_SET);
  float arr[line_count];

  int i = 0;
  while (1) {
    scan_ret = fscanf(read_pointer, "%f", &diff);

    if (scan_ret == EOF) {
      break;
    }
    arr[i] = diff;
    i++;
  }

  float* sorted_arr = malloc(line_count * sizeof(float));
  sorted_arr = array_sort(arr, line_count);

  float arr_sum = 0.0;
  for (int i = 0; i < NUM_SCORES; i++) {
    arr_sum += sorted_arr[i];
  }

  float index = arr_sum / NUM_SCORES;

  fclose(read_pointer);
  read_pointer = NULL;

  return index;
} // fetch_eighteen_handicap_index


void swap(float* a, float* b) {
  float temp = *a;
  *a = *b;
  *b = temp;
} // swap()

float* array_sort(float* arr, int size) {
  for (int i = 0; i < size - 1; i++) {
    for (int j = 0; j < size - i - 1; j++) {
      if (arr[j] > arr[j + 1]) {
        swap(&arr[j], &arr[j + 1]);
      }
    }
  }
  return arr;
} // array_sort()

void print_arr(float* arr, int size) {
  printf("\n");
  for (int i = 0; i < size; i++) {
    printf("%1f ", arr[i]);
  }
}
