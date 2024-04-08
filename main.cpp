// #include <stdio.h>
// #include <math.h>
// #include <string>
// #include <iostream>
// #include <fstream>
// #include <vector>
// #include <memory>
// #include <functional>
// #include <unistd.h>
// #include <chrono>
// #include <assert.h>

// #include <NvInfer.h>
// #include <NvOnnxParser.h>
// #include <NvInferRuntime.h>


// using namespace nvinfer1;
// // logger用来管控打印日志级别
// // TRTLogger继承自nvinfer1::ILogger
// class TRTLogger : public nvinfer1::ILogger
// {
//     void log(Severity severity, const char *msg) noexcept override
//     {
//         // 屏蔽INFO级别的日志
//         if (severity != Severity::kINFO)
//             std::cout << msg << std::endl;
//     }
// } gLogger;

// // 加载模型
// std::vector<unsigned char> loadEngineModel(const std::string &fileName)
// {
//     std::ifstream file(fileName, std::ios::binary);        // 以二进制方式读取
//     assert(file.is_open() && "load engine model failed!"); // 断言

//     file.seekg(0, std::ios::end); // 定位到文件末尾
//     size_t size = file.tellg();   // 获取文件大小

//     std::vector<unsigned char> data(size); // 创建一个vector，大小为size
//     file.seekg(0, std::ios::beg);          // 定位到文件开头
//     file.read((char *)data.data(), size);  // 读取文件内容到data中
//     file.close();

//     return data;
// }


// int main(int argc, char **argv)
// {

//     // TRTLogger logger;
//     // nvinfer1::IRuntime *runtime = nvinfer1::createInferRuntime(logger);

//     // // ==================== 2. 反序列化生成engine ====================
//     // // 读取文件
//     // auto engineModel = loadEngineModel("./MNIST.engine");
//     // // 调用runtime的反序列化方法，生成engine，参数分别是：模型数据地址，模型大小，pluginFactory
//     // nvinfer1::ICudaEngine *engine = runtime->deserializeCudaEngine(engineModel.data(), engineModel.size());

//     // if (!engine)
//     // {
//     //     std::cout << "deserialize engine failed!" << std::endl;
//     //     return -1;
//     // }

//     // // ==================== 3. 创建一个执行上下文 ====================
//     // nvinfer1::IExecutionContext *context = engine->createExecutionContext();
//     // // 设置stream 流
//     // cudaStream_t stream = nullptr;
//     // cudaStreamCreate(&stream);

//     // // 数据流转：host --> device ---> inference ---> host

//     // // 输入数据
//     // float *host_input_data = new float[3]{2, 4, 8}; // host 输入数据
//     // int input_data_size = 3 * sizeof(float);        // 输入数据大小
//     // float *device_input_data = nullptr;             // device 输入数据

//     // // 输出数据
//     // float *host_output_data = new float[2]{0, 0}; // host 输出数据
//     // int output_data_size = 2 * sizeof(float);     // 输出数据大小
//     // float *device_output_data = nullptr;          // device 输出数据

//     // // 申请device内存
//     // cudaMalloc((void **)&device_input_data, input_data_size);
//     // cudaMalloc((void **)&device_output_data, output_data_size);

//     // // host --> device
//     // // 参数分别是：目标地址，源地址，数据大小，拷贝方向
//     // // 使用异步拷贝，提高利用率
//     // cudaMemcpyAsync(device_input_data, host_input_data, input_data_size, cudaMemcpyHostToDevice, stream);

//     // // bindings告诉Context输入输出数据的位置
//     // float *bindings[] = {device_input_data, device_output_data};

//     // // ==================== 5. 执行推理 ====================
//     // bool success = context -> enqueueV3(stream);
//     // // 数据从device --> host
//     // cudaMemcpyAsync(host_output_data, device_output_data, output_data_size, cudaMemcpyDeviceToHost, stream);
//     // // 等待流执行完毕
//     // cudaStreamSynchronize(stream);
//     // // 输出结果
//     // std::cout << "输出结果: " << host_output_data[0] << " " << host_output_data[1] << std::endl;

//     // // ==================== 6. 释放资源 ====================
//     // cudaStreamDestroy(stream);
//     // cudaFree(device_input_data); 
//     // cudaFree(device_output_data);

//     // delete host_input_data;
//     // delete host_output_data;

//     // delete context;
//     // delete engine;
//     // delete runtime;

//      // 实例化ILogger
//     Logger logger;

//     // 创建runtime
//     auto runtime = unique_ptr<IRuntime>(createInferRuntime(logger));

//     // 读取engine,反序列化
//     string file_path = "MNIST.engine";
//     auto plan = load_engine_file(file_path);
//     auto engine = shared_ptr<ICudaEngine>(runtime->deserializeCudaEngine(plan.data(), plan.size()));

//     // 创建执行上下文
//     auto context = unique_ptr<IExecutionContext>(engine->createExecutionContext());

//     auto idims = engine->getTensorShape("Input3");// 这里的名字可以在导出时修改
//     std::cout<<idims<<std::endl;
//     // auto odims = engine->getTensorShape("Plus214_Output_0");
//     // Dims4 inputDims = { 1, idims.d[1], idims.d[2], idims.d[3] };
//     // Dims2 outputDims = { 1, 10 };
//     // context->setInputShape("Input3", inputDims);

//     // void* buffers[2];
//     // const int inputIndex = 0;
//     // const int outputIndex = 1;

//     // cudaMalloc(&buffers[inputIndex], 1 * 28 * 28 * sizeof(float));
//     // cudaMalloc(&buffers[outputIndex], 10 * sizeof(float));

//     // // 设定数据地址
//     // context->setTensorAddress("Input3", buffers[inputIndex]);
//     // context->setTensorAddress("Plus214_Output_0", buffers[outputIndex]);

//     // // 创建cuda流
//     // cudaStream_t stream;
//     // cudaStreamCreate(&stream);

//     return 0;
// }


#include <stdio.h>
#include <math.h>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <memory>
#include <functional>
#include <unistd.h>
#include <chrono>
#include <assert.h>

#include <NvInfer.h>
#include <NvOnnxParser.h>
#include <NvInferRuntime.h>


using namespace std;
using namespace nvinfer1;

// 以下示例捕获所有警告消息，但忽略信息性消息
class Logger : public ILogger           
{
    void log(Severity severity, const char* msg) noexcept override
    {
        // 抑制信息级别的消息
        if (severity <= Severity::kWARNING)
            cout << msg << endl;
    }
};

// 加载模型文件
std::vector<unsigned char> load_engine_file(const std::string &file_name)
{
    std::vector<unsigned char> engine_data;
    std::ifstream engine_file(file_name, std::ios::binary);
    assert(engine_file.is_open() && "Unable to load engine file.");
    engine_file.seekg(0, engine_file.end);
    int length = engine_file.tellg();
    engine_data.resize(length);
    engine_file.seekg(0, engine_file.beg);
    engine_file.read(reinterpret_cast<char *>(engine_data.data()), length);
    return engine_data;
}

int softmax(const float(&rst)[10]){
    float cache = 0;
    int idx = 0;
    for(int i = 0; i < 10; i += 1)
    {
        if(rst[i]>cache)
        {
            cache = rst[i];
            idx = i;
        };
    };
    return idx;
}

int main(int argc, char **argv)
{
    // 实例化ILogger
    Logger logger;

    // 创建runtime
    auto runtime = unique_ptr<IRuntime>(createInferRuntime(logger));

    // 读取engine,反序列化
    string file_path = "MNIST.engine";
    auto plan = load_engine_file(file_path);
    auto engine = shared_ptr<ICudaEngine>(runtime->deserializeCudaEngine(plan.data(), plan.size()));

    // 创建执行上下文
    auto context = unique_ptr<IExecutionContext>(engine->createExecutionContext());

    auto idims = engine->getTensorShape("Input3");// 这里的名字可以在导出时修改
    auto odims = engine->getTensorShape("Plus214_Output_0");
    Dims4 inputDims = { 1, idims.d[1], idims.d[2], idims.d[3] };
    Dims2 outputDims = { 1, 10 };
    context->setInputShape("Input3", inputDims);

    void* buffers[2];
    const int inputIndex = 0;
    const int outputIndex = 1;

    cudaMalloc(&buffers[inputIndex], 1 * 28 * 28 * sizeof(float));
    cudaMalloc(&buffers[outputIndex], 10 * sizeof(float));

    // 设定数据地址
    context->setTensorAddress("Input3", buffers[inputIndex]);
    context->setTensorAddress("Plus214_Output_0", buffers[outputIndex]);

    // 创建cuda流
    cudaStream_t stream;
    cudaStreamCreate(&stream);

    // 读取文件执行推理
    for(int i = 0; i < 10; i += 1)
    {
        // 读取图片
        // cv::Mat img0;
        // std::string file_name = "img/img" + std::to_string(i) + ".png";
        // img0 = cv::imread(file_name, 0);// 0为灰度图片
        // if (img0.empty())  //检测image有无数据，无数据 image.empty()返回 真
        // {
        //     std::cout << "Could not open or find the image" << std::endl;
        //     return -1;
        // }
        // cv::Mat img;
        // img0.convertTo(img, CV_32F);
        // cv::imshow(file_name,img);
        // cv::waitKey(0);
        float img[28*28] = {0};
        // 将图像拷贝到GPU
        cudaMemcpyAsync(buffers[inputIndex], img, 1 * 28 * 28 * sizeof(float), cudaMemcpyHostToDevice, stream);

        //执行推理
        context->enqueueV3(stream);
        cudaStreamSynchronize(stream);

        float rst[10];
        cudaMemcpyAsync(&rst, buffers[outputIndex], 1 * 10 * sizeof(float), cudaMemcpyDeviceToHost, stream);
 
        cout << " 推理结果: " << softmax(rst) <<endl;
    }

    cudaStreamDestroy(stream);
    cudaFree(buffers[inputIndex]);
    cudaFree(buffers[outputIndex]);
}