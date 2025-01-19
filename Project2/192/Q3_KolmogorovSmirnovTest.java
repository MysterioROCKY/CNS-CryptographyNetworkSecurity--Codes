import java.util.Arrays;
import java.util.Random;

public class Q3_KolmogorovSmirnovTest {

    // Method to perform KS test
    public static void ksTest(int seed) {
        int N = 30;
        Random rand = new Random(seed);
        double[] data = new double[N];

        // Generate random numbers
        for (int i = 0; i < N; i++) {
            data[i] = 5 * rand.nextDouble();  // Ufloat[0, 5]
        }

        // Sort the data
        Arrays.sort(data);

        // Calculate the empirical CDF
        double[] ecdf = new double[N];
        for (int i = 0; i < N; i++) {
            ecdf[i] = (i + 1) / (double) N;
        }

        // Calculate the theoretical CDF (uniform U[0, 5])
        double[] cdf = new double[N];
        for (int i = 0; i < N; i++) {
            cdf[i] = data[i] / 5.0;
        }

        // Calculate KS test statistic
        double ksStatistic = 0;
        for (int i = 0; i < N; i++) {
            double diff1 = Math.abs(ecdf[i] - cdf[i]);
            double diff2 = Math.abs((i / (double) N) - cdf[i]);
            double diff3 = Math.abs(ecdf[i] - ((i + 1) / (double) N));
            ksStatistic = Math.max(ksStatistic, Math.max(diff1, Math.max(diff2, diff3)));
        }

        // Critical values for 95% and 99% confidence intervals
        double chiCritical95 = 1.36 / Math.sqrt(N);
        double chiCritical99 = 1.63 / Math.sqrt(N);

        // Conclusion
        String conclusion;
        if (ksStatistic > chiCritical99) {
            conclusion = "Not Random";
        } else if (ksStatistic > chiCritical95) {
            conclusion = "Random (95% confidence)";
        } else {
            conclusion = "Random (99% confidence)";
        }

        // Print results
        System.out.printf("Seed: %d\n", seed);
        System.out.printf("KS Statistic = %.4f\n", ksStatistic);
        System.out.printf("95%% Confidence Threshold = %.4f\n", chiCritical95);
        System.out.printf("99%% Confidence Threshold = %.4f\n", chiCritical99);
        System.out.printf("Conclusion: %s\n\n", conclusion);
    }

    public static void main(String[] args) {
        // Print OS and kernel version
        System.out.println("Operating System: " + System.getProperty("os.name"));
        System.out.println("Kernel Version: " + System.getProperty("os.version"));
        System.out.println();

        // Perform KS test for 13 runs with different seeds
        int[] seeds = {123, 456, 789, 234, 567, 890, 345, 678, 901, 432, 765, 109, 654};
        for (int seed : seeds) {
            ksTest(seed);
        }
    }
}
