# 工科賽電腦修護第二站 - 環境設置

## 為什麼選擇 VSCode 進行 Arduino 開發？

在這次的專案中，我們選擇使用 Visual Studio Code (VSCode) 來進行 Arduino 開發，而不是傳統的 Arduino IDE。這個選擇基於以下幾個考量：

1. 更強大的編輯功能：VSCode 提供了進階的程式碼編輯功能，如智慧程式碼補全和更好的語法標明，這可以提高編碼效率。

2. 豐富的擴充生態：VSCode 有大量可用的擴充功能，可以根據需求添加各種功能，使開發環境更加靈活。

3. 更好的版本控制：VSCode 與 Git 等版本控制系統有良好的整合，這對於管理專案程式碼很有幫助。

雖然 VSCode 的初始設置可能比 Arduino IDE 稍微複雜一些，但長遠來看，它能夠提供更高效的開發體驗。

接下來，讓我們看看如何設置 VSCode 環境。

## VSCode 中的 Arduino 環境設置 (Windows)

1. 安裝 Arduino CLI

    - 從 [Arduino CLI 官網](https://arduino.github.io/arduino-cli/) 下載 Windows msi 安裝檔

    - 安裝完成後，確保 Arduino CLI 的路徑 `C:/Program Files/Arduino CLI/` 已加入系統的 `PATH` 環境變數中

2. 在 VSCode 中設置 Arduino

    - 安裝 Arduino 擴充套件：

        - 開啟 VSCode，點擊左側的擴充功能圖示（或使用快捷鍵 `Ctrl+Shift+X`）

        - 搜尋 "Arduino"，找到並安裝 Microsoft 提供的 "Arduino" 擴充功能

    - 配置 Arduino 設定：

        - 開啟 VSCode 設定（使用快捷鍵 `Ctrl+,`），搜尋 "Arduino"

        - 設定以下選項：

            - `Arduino: Command Path`: 填入 `arduino-cli.exe`

            - `Arduino: Path`: 填入 `C:/Program Files/Arduino CLI/`

            - `Arduino: Use Arduino CLI`: 勾選此選項

3. 安裝 Arduino Core 和必要函式庫

    - 在 VSCode 中，開啟命令選擇區（使用快捷鍵 `Ctrl+Shift+P`）

    - 輸入並選擇 `Arduino: Board Manager`

    - 搜尋並安裝 `Arduino AVR Boards`

    - 再次開啟命令選擇區，輸入並選擇 `Arduino: Library Manager`

    - 搜尋並安裝 `Adafruit_SSD1306` 和其他需要的函式庫

## 電腦端應用程式開發環境設置

1. 安裝 Visual Studio

    - 前往 [Visual Studio 下載頁面](https://visualstudio.microsoft.com/downloads/)

    - 下載並安裝 Visual Studio（建議使用 Community 版本，它是免費的）

    - 在安裝過程中，確保選擇 ".NET 桌面開發" 工作負載

2. 建立新專案

    - 開啟 Visual Studio

    - 選擇 "建立新專案"

    - 選擇 "Windows Forms 應用程式 (.NET Framework)" 或 "WPF 應用程式 (.NET Framework)"，根據您預期的 UI 框架而定

完成以上步驟後，您的開發環境就已經準備就緒，可以開始進行 Arduino 程式和電腦端 C# 應用程式的開發了。
