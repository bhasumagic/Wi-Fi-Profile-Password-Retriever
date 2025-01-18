# Wi-Fi Profile Password Retriever (Multithreaded in C++)

This project retrieves stored Wi-Fi profile names and their passwords (if available) from the system using multithreading in C++. It interacts with system-level commands (`netsh`) to fetch profile details and parses the output to extract relevant information, writing the results to a file.

## Features
- Retrieves all stored Wi-Fi profiles on a Windows system.
- Extracts and displays passwords for profiles where available.
- Uses **multithreading** to speed up the retrieval process, dividing the workload across multiple threads.
- Implements **thread-safe file writing** using `std::mutex` to avoid race conditions.
- Clean and modular design:
  - **`makeList`:** Fetches and parses Wi-Fi profile names.
  - **`writePass`:** Extracts passwords for a given profile and appends them to a file.
  - **`batch`:** Manages a range of profiles for concurrent processing by threads.

## How It Works
1. The program runs the `netsh wlan show profiles` command to list stored Wi-Fi profiles.
2. For each profile, it executes the `netsh wlan show profile name="PROFILE_NAME" key=clear` command to fetch details, including the password if available.
3. Passwords (or a note if the password is hidden) are written to a file named `passwords.txt`.
4. Multithreading divides the workload, making the process faster and more efficient.

## Usage
1. Clone the repository:
   ```bash
   git clone https://github.com/yourusername/your-repo-name.git
