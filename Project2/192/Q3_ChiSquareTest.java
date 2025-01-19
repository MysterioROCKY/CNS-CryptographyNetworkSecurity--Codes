import java.util.Random;

public class Q3_ChiSquareTest {

    // Method to perform Chi-Square test
    public static void chiSquareTest(int seed) {
        int numSamples = 5000;
        int numBins = 1000;
        Random rand = new Random(seed);
        int[] data = new int[numSamples];

        // Generate random integers
        for (int i = 0; i < numSamples; i++) {
            data[i] = rand.nextInt(numBins) + 1;  // Uint(1, 1000)
        }

        // Calculate observed frequencies
        int[] observedCounts = new int[numBins];
        for (int value : data) {
            observedCounts[value - 1]++;
        }

        // Expected frequency for uniform distribution
        double expectedFrequency = numSamples / (double) numBins;

        // Calculate Chi-Square statistic
        double chiSquareStatistic = 0;
        for (int count : observedCounts) {
            double diff = count - expectedFrequency;
            chiSquareStatistic += (diff * diff) / expectedFrequency;
        }

        // Chi-square critical values for 999 degrees of freedom
        double chiCritical95 = chiSquareCriticalValue(numBins - 1, 0.95);
        double chiCritical99 = chiSquareCriticalValue(numBins - 1, 0.99);

        // Conclusion
        String conclusion;
        if (chiSquareStatistic < chiCritical95) {
            conclusion = "Random (95% confidence)";
        } else if (chiSquareStatistic < chiCritical99) {
            conclusion = "Not Random (95% confidence) but Random (99% confidence)";
        } else {
            conclusion = "Not Random (99% confidence)";
        }

        // Print results
        System.out.printf("Seed %d:\n", seed);
        System.out.printf("  Chi-Square Statistic = %.2f\n", chiSquareStatistic);
        System.out.printf("  Chi-Square Table Value (95%% CI) = %.2f, (99%% CI) = %.2f\n", chiCritical95, chiCritical99);
        System.out.printf("  Conclusion: %s\n\n", conclusion);
    }

    // Method to get Chi-Square critical value for given degrees of freedom and confidence level
    private static double chiSquareCriticalValue(int degreesOfFreedom, double confidenceLevel) {
        // Approximate critical values for demonstration purposes
        // For exact values, use a statistical library or precomputed table
        if (confidenceLevel == 0.95) {
            if (degreesOfFreedom == 1) return 3.841;
            if (degreesOfFreedom == 2) return 5.991;
            if (degreesOfFreedom == 999) return 1051.39;

            // Approximate for other degrees of freedom
            return degreesOfFreedom + Math.sqrt(2 * degreesOfFreedom);
        } else if (confidenceLevel == 0.99) {
            if (degreesOfFreedom == 1) return 6.635;
            if (degreesOfFreedom == 2) return 9.210;
            if (degreesOfFreedom == 999) return 1180.79;

            // Approximate for other degrees of freedom
            return degreesOfFreedom + 2 * Math.sqrt(degreesOfFreedom);
        }
        return 0; // Placeholder
    }

    public static void main(String[] args) {
        // Print OS and kernel version
        System.out.println("Operating System: " + System.getProperty("os.name"));
        System.out.println("Kernel Version: " + System.getProperty("os.version"));
        System.out.println();

        // Perform Chi-Square test for 13 runs with different seeds
        int[] seeds = {123, 456, 789, 234, 567, 890, 345, 678, 901, 432, 765, 109, 654};
        for (int seed : seeds) {
            chiSquareTest(seed);
        }
    }
}

