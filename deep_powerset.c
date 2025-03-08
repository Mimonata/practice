/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   deep_powerset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spitul <spitul@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/22 07:40:10 by spitul            #+#    #+#             */
/*   Updated: 2025/02/22 07:46:19 by spitul           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <math.h>

// Function to print subsets that sum up to the given target sum
void findSubsetsWithSum(int arr[], int n, int targetSum) {
    int totalSubsets = 0;

    // Total number of subsets is 2^n
    int total = pow(2, n);

    // Iterate through all possible subsets
    for (int i = 0; i < total; i++) {
        int sum = 0;
        // Check each element in the subset
        for (int j = 0; j < n; j++) {
            // If the j-th bit is set, include arr[j] in the subset
            if (i & (1 << j)) {
                sum += arr[j];
            }
        }
        // If the sum matches the target sum, print the subset
        if (sum == targetSum) {
            printf("Subset %d: ", ++totalSubsets);
            for (int j = 0; j < n; j++) {
                if (i & (1 << j)) {
                    printf("%d ", arr[j]);
                }
            }
            printf("\n");
        }
    }

    if (totalSubsets == 0) {
        printf("No subset found with sum = %d\n", targetSum);
    } else {
        printf("Total subsets with sum = %d: %d\n", targetSum, totalSubsets);
    }
}

int main() {
    int arr[] = {300, -100, 200, 400};
    int n = sizeof(arr) / sizeof(arr[0]);
    int targetSum = 800;

    findSubsetsWithSum(arr, n, targetSum);

    return 0;
}
