import random 
import numpy as np
from scipy.stats import chi2
import platform

# Function for Chi-Square Test
def chi_square_test():
    num_samples = 5000
    seeds = [123, 456, 789, 234, 567, 890, 345, 678, 901, 432, 765, 109, 654]  # Fixed 13 seeds
    expected_count = num_samples / 1000  # Expected frequency for uniform distribution

    print("----- Chi-Square Test Results -----")
    for i, seed in enumerate(seeds):
        random.seed(seed)
        data = [random.randint(1, 1000) for _ in range(num_samples)]
        
        observed_counts = np.histogram(data, bins=1000, range=(1, 1000))[0]
        chi_statistic = ((observed_counts - expected_count) ** 2 / expected_count).sum()
        
        # Chi-square critical values for 999 degrees of freedom (1000-1 bins)
        chi_critical_95 = chi2.ppf(0.95, df=999)
        chi_critical_99 = chi2.ppf(0.99, df=999)
        
        print(f"Run {i+1}:")
        print(f"  Chi-square Statistic = {chi_statistic:.2f}")
        print(f"  Chi-square Table Value (95% CI) = {chi_critical_95:.2f}, (99% CI) = {chi_critical_99:.2f}")
        
        if chi_statistic < chi_critical_99:
            conclusion = "Random (99% confidence)"
        elif chi_statistic < chi_critical_95:
            conclusion = "Random (95% confidence)"
        else:
            conclusion = "Not Random"
        
        print(f"  Conclusion: {conclusion}")
        print()

# Function for KS Test with critical value comparison
def ks_test():
    seeds = [123, 456, 789, 234, 567, 890, 345, 678, 901, 432, 765, 109, 654]  # Fixed 13 seeds

    print("----- Kolmogorov-Smirnov (KS) Test Results -----")
    for i, seed in enumerate(seeds):
        random.seed(seed)
        data = [random.uniform(0, 5) for _ in range(30)]
        
        # Sort the data for KS test
        data.sort()

        # Compute KS statistic
        n = len(data)
        ks_stat = 0.0
        for j in range(n):
            empirical_cdf = (j + 1) / n
            theoretical_cdf = data[j] / 5.0  # Uniform distribution U[0,5]
            ks_stat = max(ks_stat, abs(empirical_cdf - theoretical_cdf))

        # Critical values for KS test at 95% and 99% confidence
        ks_critical_95 = 1.36 / np.sqrt(n)
        ks_critical_99 = 1.63 / np.sqrt(n)

        # Conclusion based on KS statistic comparison with critical values
        print(f"Run {i+1}:")
        print(f"  KS-statistic = {ks_stat:.4f}")
        print(f"  KS Table Value (95% CI) = {ks_critical_95:.4f}, (99% CI) = {ks_critical_99:.4f}")
        
        if ks_stat < ks_critical_99:
            conclusion = "Random (99% confidence)"
        elif ks_stat < ks_critical_95:
            conclusion = "Random (95% confidence)"
        else:
            conclusion = "Not Random"
        
        print(f"  Conclusion: {conclusion}")
        print()

# Print OS and kernel version
print("Operating System:", platform.system())
print("Kernel Version:", platform.release())
print(" ")

# Run the tests
chi_square_test()
ks_test()
