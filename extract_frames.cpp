#include <opencv2/opencv.hpp>
#include <iostream>
#include <filesystem>

class app_extractFrame
{
    private:
        void extractFrames(const std::string& videoPath, const std::string& outputFolder, double interval);
        void processVideos(void);

        std::string inputFolder;    // 替换为你的视频文件夹路径
        std::string outputFolder;   // 替换为你希望保存帧的文件夹
        double interval = 10.0;      // 默认抽帧间隔（单位：秒）

    public:
        app_extractFrame(void) {
            inputFolder  = std::filesystem::current_path().string(); // 获取当前工作目录
            outputFolder = std::filesystem::current_path().string(); // 获取当前工作目录
        }
        app_extractFrame(std::string& InputFolder, std::string& OutputFolder, double Interval = 10.0):inputFolder(InputFolder),outputFolder(OutputFolder),interval(Interval) {}
        void run(void) {
            if (inputFolder.empty()) std::cout << "Input folder path is not set!" << std::endl;
            else processVideos();
        }
};



void app_extractFrame::extractFrames(const std::string& videoPath, const std::string& outputFolder, double interval) {
    cv::VideoCapture cap(videoPath);
    if (!cap.isOpened()) {
        std::cerr << "Error: Cannot open video file " << videoPath << std::endl;
        return;
    }

    double fps = cap.get(cv::CAP_PROP_FPS);
    int frameInterval = static_cast<int>(fps * interval);
    cv::Mat frame;
    int count = 0;
    int savedCount = 0;

    while (true) {
        if (!cap.read(frame)) {
            break;  // 读取结束
        }

        if (count % frameInterval == 0) {
            std::string outputPath = outputFolder + "/" + std::filesystem::path(videoPath).filename().string() + "_frame_" + std::to_string(savedCount) + ".jpg";
            cv::imwrite(outputPath, frame);
            savedCount++;
        }

        count++;
    }

    cap.release();
    std::cout << "Extracted " << savedCount << " frames from " << videoPath << std::endl;
}

void app_extractFrame::processVideos(void) {
    std::cout << "Input folder: " << inputFolder << std::endl;  //视频输入文件夹路径
    std::cout << "Output folder: " << outputFolder << std::endl;//图片输出文件夹路径
    std::cout << "Frame interval: " << interval << " seconds" << std::endl;//抽帧间隔
    std::cout << "Processing videos..." << std::endl;//处理视频

    std::filesystem::create_directories(outputFolder);  // 创建输出目录

    for (const auto& entry : std::filesystem::recursive_directory_iterator(inputFolder)) {
        if (entry.is_regular_file() && (entry.path().extension() == ".mp4" || entry.path().extension() == ".avi" || entry.path().extension() == ".mov" || entry.path().extension() == ".mkv")) {
            extractFrames(entry.path().string(), outputFolder, interval);
        }
    }
    std::cout << "The Frame has been saved in \"" << outputFolder << '\"' << std::endl;
}



int main(int argc, char** argv) {
    // std::cout <<"Video I/0:"<<cv::getBuildInformation()<< std::endl; // 输出OpenCV编译信息

    std::string inputFolder;  // 视频文件夹路径
    std::string outputFolder;  // 你希望保存帧的文件夹
    double interval = 10.0;     // 抽帧间隔（单位：秒）
    if (argc == 4) {//(有输入输出目录与间隔)
        inputFolder  = argv[1];  // 替换为你的视频文件夹路径
        outputFolder = argv[2];  // 替换为你希望保存帧的文件夹
        interval = std::stod(argv[3]);  // 抽帧间隔（单位：秒）
    }
    else if (argc == 1) {
        inputFolder  = ".\\Video";  // 视频文件夹路径
        outputFolder = ".\\FrameWork";  // 替换为你希望保存帧的文件夹
        interval = 5;  // 抽帧间隔（单位：秒）
    }

    app_extractFrame app(inputFolder, outputFolder, interval);
    
    app.run();
    
    return 0;
}
