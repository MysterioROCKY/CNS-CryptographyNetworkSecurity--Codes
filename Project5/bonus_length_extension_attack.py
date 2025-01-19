"""
How to Run the code :->

Step 1) Install "Python 3.7" or lower to work correctly with "hashpumpy" 
Step 2) In terminal run command : python3 bonus_length_extension_attack.py

Output: 
Original hash (H(secret || data)):  c56533be56d8463942e1a402c59e3b61
Secret length in bytes:  53

Extended hash (H(secret || data || append)):  32c369cfe32322041bb519a0fe6c736d
Extended message (data || padding || append):  'My full name is Sanyam Agrawal.\x80\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\xa0\x02\x00\x00\x00\x00\x00\x00SE21UCSE192'

Verification hash (recomputed with secret):  32c369cfe32322041bb519a0fe6c736d

Length extension attack successful!
"""

import hashpumpy
import hashlib

# Secret, data, and append values (from your assignment requirements)
secret = "Hello, World!. Message extension attacks. SE21UCSE192"
data = "My full name is Sanyam Agrawal."
append = "SE21UCSE192"

# Step 1: Compute original hash (H(secret || data))
original_message = secret + data
original_hash = hashlib.md5(original_message.encode()).hexdigest()

# Dynamically calculate the secret's length in bytes
original_secret_length = len(secret.encode())

print("Original hash (H(secret || data)): ", original_hash)
print("Secret length in bytes: ", original_secret_length)

# Step 2: Perform the length extension attack
try:
    # Perform the attack with hashpumpy
    new_hash, extended_message = hashpumpy.hashpump(
        original_hash,      # Hash of H(secret || data)
        data,               # Original data
        append,             # Data to append
        original_secret_length  # Secret length in bytes
    )

    # Convert the extended message to a string
    # Hashpumpy adds padding automatically to match MD5 behavior
    extended_message_str = extended_message.decode('latin1')

    # Step 3: Print results of the attack
    print("\nExtended hash (H(secret || data || append)): ", new_hash)
    print("Extended message (data || padding || append): ", repr(extended_message_str))

    # Step 4: Verification (simulating server-side hash with the actual secret)
    # Compute the hash directly with the full message (including secret + padding + append)
    # Hashpumpy provides the padding already, so we just need to concatenate `secret` + `extended_message`
    full_message = secret.encode() + extended_message
    verification_hash = hashlib.md5(full_message).hexdigest()

    print("\nVerification hash (recomputed with secret): ", verification_hash)

    # Compare the new hash with the verification hash
    if new_hash == verification_hash:
        print("\nLength extension attack successful!")
    else:
        print("\nLength extension attack failed.")

except Exception as e:
    print(f"Error during hash extension attack: {e}")
