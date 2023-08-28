#include <windows.h>

int main() {
    // Replace "C:/path/to/your/file.txt" with the actual file path
    char* filePath = "C:/Users/aadiu/Pictures/Screenshots/Screenshot (15).png";

    // Use ShellExecuteW to open the specific file
    ShellExecuteA(NULL, "open", filePath, NULL, NULL, SW_SHOWNORMAL);

    return 0;
}