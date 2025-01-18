#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <thread>
#include <mutex>

#define NUM_THREADS 5

std::mutex P_mutex;

FILE* run(const char cmd[])
{
    FILE* pipe = popen(cmd, "r");
    if (!pipe)
        exit(1);

    return pipe;
}

std::vector<std::string> makeList(FILE* text_stream)
{
    std::vector<std::string> profiles;
    char buffer[128];

    while(fgets(buffer, 128, text_stream))
    {
        std::string line(buffer);

        if (line.find("All User Profile") != std::string::npos)
        {
            size_t start = line.find(":") + 2;
            size_t end = line.find("\n");
            profiles.push_back(line.substr(start, end - start));
        }
    }

    return profiles;
}

void writePass(const std::string& profile, std::ofstream& file)
{
    std::string cmd = "netsh wlan show profile name=\"" + profile + "\" key=clear";
    FILE* pipe = run(cmd.c_str());

    std::string password;
    char buffer[128];

    while (fgets(buffer, 128, pipe))
    {
        std::string line(buffer);

        if (line.find("Key Content") != std::string::npos)
        {
            size_t start = line.find(":") + 2;
            size_t end = line.find("\n");
            password = line.substr(start, end - start);
        }
    }

    P_mutex.lock();

    if (password.empty())
        file << profile << " : Password is hidden! " << std::endl;
    else
        file << profile << " : " << password << std::endl;

    P_mutex.unlock();

    pclose(pipe);
}

void batch(int start, int end, std::vector<std::string>& profiles, std::ofstream& file)
{
    for (int i = start; i < end; i++)
        writePass(profiles[i], file);
}

int main()
{
    std::vector<std::string> profiles = makeList(run("netsh wlan show profiles"));
    int size = profiles.size();

    std::ofstream file("passwords.txt");

    std::thread threads[NUM_THREADS];

    for (int i = 0; i < NUM_THREADS; i++)
        threads[i] = std::thread(batch, i * size / NUM_THREADS, (i + 1) * size / NUM_THREADS, std::ref(profiles), std::ref(file));

    for (int i = 0; i < NUM_THREADS; i++)
        threads[i].join();

}