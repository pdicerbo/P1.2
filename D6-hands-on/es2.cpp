/**************************************************************************/
/* Simple program in OpenCL that calls to a kernel to sum two vectors     */
/* not using unified memories (with copies):                              */ 
/*        clCreateBuffer + CL_MEM_USE_HOST_PTR                            */
/* It outputs the time in seconds to create the buffers to finish the     */
/* kernel execution.                                                      */
/**************************************************************************/


#include <fstream>
#include <sstream>
#include <CL/cl.h>
#include "common.hpp"

double times;

cl_int            clError     = CL_SUCCESS;
cl_int            err         = CL_SUCCESS;
cl_platform_id    platform_id = 0;
cl_device_id      device; 
cl_event          event       = 0;
cl_context        context;
cl_program        program;
cl_command_queue  commandQueue;
cl_kernel         kernel      = 0;

void cleanUpOpenCL(void) {

    clReleaseContext(context);
    clReleaseCommandQueue(commandQueue);
    clReleaseProgram(program);
    return ;
}

inline bool checkSuccess(bool err) {

    return err == CL_SUCCESS;
}

inline void checkErr(cl_int err, const char * name) {

    if (err != CL_SUCCESS) {
        std::cerr << "ERROR: " << name
        << " (" << err << ")" << std::endl;
        exit(EXIT_FAILURE);
    }
}

void initOpenCL(void) {
    
    /* get the platform(s) */
    cl_uint num_platforms;
    clError = clGetPlatformIDs( 0, NULL, &num_platforms );

    checkErr (clError, "clGetPlatformIDs( 0, NULL, &num_platforms );");
    if (num_platforms <= 0) {
        std::cerr << "No platform..." << std::endl;
        exit(1);
    }
    
    cl_platform_id* platforms = new cl_platform_id[num_platforms];
    clError = clGetPlatformIDs(num_platforms, platforms, NULL);
    checkErr(clError, "clGetPlatformIDs( num_platforms, &platforms, NULL );");
    platform_id = platforms[0];
    delete platforms;

    /* Connect to a compute device */
    err = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_ALL, 1, &device, NULL);
    checkErr(err,"Failed to create a device group");

    /* Create a compute context */
    context = clCreateContext(NULL, 1, &device, NULL, NULL, &err);
    checkErr(err,"Failed to create a compute context!");

    /* Create a command queue */
    commandQueue = clCreateCommandQueue(context, device, 0, &err);
    checkErr(err,"Failed to create a command commands!");

    /* Open Kernel file */
    std::string filename = "/home/ictp06/session-3/kernels.cl";
    std::ifstream kernelFile(filename.c_str(), std::ios::in);
    
    if (not kernelFile.is_open()) {
        std::cout << "Unable to open " << filename << ". " << std::endl;
        exit(1);
    }
    
    /*
     * Read the kernel file into an output stream.
     * Convert this into a char array for passing to OpenCL.
     */
    std::ostringstream outputStringStream;
    outputStringStream << kernelFile.rdbuf();
    std::string srcStdStr = outputStringStream.str();
    const char* charSource = srcStdStr.c_str();
    
    kernelFile.close();
    /* Create the compute program from the source buffer */
    program = clCreateProgramWithSource(context, 1, 
               (const char **) &charSource, NULL, &err);
    if (not program) {
        std::cerr << "Error: Failed to create compute program!" << std::endl;
        exit(1);
    }
    
    /* Build the program executable */
    const char* flags = "";


    err = clBuildProgram(program, 0, NULL, flags, NULL, NULL);
    if (err != CL_SUCCESS) {
        size_t len;
        char buffer[1024];
    
        std::cerr << "Error: Failed to build program executable!" 
                  << std::endl;
        clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, 
                sizeof(buffer), buffer, &len);
        std::cerr << buffer << std::endl;
        exit(1);
    }

    /* Create Kernels objects for all the kernels in the OpenCL program */
    cl_uint num_kernels;
    kernel = clCreateKernel(program, "vector_sum", &err);
    if (err != CL_SUCCESS) {
        size_t len;
        char buffer[1024];
    
        std::cout << "Error: Failed to create kernels in program!" 
                  << std::endl;
        clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, 
                sizeof(buffer), buffer, &len);
        std::cout << buffer << std::endl;
        exit(1);
    }
}


inline void vector_sum(const int  arraySize, 
                       const double* inputA, 
                       const double* inputB, 
                             double* output)
{ 
    /* Allocate memory buffers */
    /*
    * Ask the OpenCL implementation to allocate buffers for the data.
    * We ask the OpenCL implemenation to allocate memory rather than 
    * allocating it on the CPU to avoid having to copy the data later.
    * The read/write flags relate to accesses to the memory from within 
    * the kernel.
    */

    bool createMemoryObjectSuccess = true;
    int numberOfMemoryObjects = 3;
    cl_mem memoryObjects[3] = {0, 0, 0};
    int errorNumber = 0;

    int bufferSize = arraySize*sizeof(double);

    memoryObjects[0] = clCreateBuffer(context, 
            CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR, 
            bufferSize, (void*)inputA, &errorNumber);
    checkErr(errorNumber, "Failed to create buffer, 1.");
    
    memoryObjects[1] = clCreateBuffer(context, 
            CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR, 
            bufferSize, (void*)inputB, &errorNumber);
    checkErr(errorNumber, "Failed to create buffer, 2.");
    
    memoryObjects[2] = clCreateBuffer(context, 
            CL_MEM_WRITE_ONLY | CL_MEM_USE_HOST_PTR, 
            bufferSize, output, &errorNumber);
    checkErr(errorNumber, "Failed to create buffer, 3.");

    /* Enqueue commands and kernels */
    /* Enqueue to the command queues the commands that control the sequence 
     * and synchronization of kernel execution, reading and writing of data,
     * and manipulation of memory objects
     */

    /* Execute a kernel function */
    /* Call clSetKernelArg() for each parameter in the kernel */
    bool setKernelArgumentsSuccess = true;
    setKernelArgumentsSuccess &= checkSuccess(clSetKernelArg(kernel, 0, 
                                        sizeof(cl_mem), &memoryObjects[0]));
    setKernelArgumentsSuccess &= checkSuccess(clSetKernelArg(kernel, 1, 
                                        sizeof(cl_mem), &memoryObjects[1]));
    setKernelArgumentsSuccess &= checkSuccess(clSetKernelArg(kernel, 2, 
                                        sizeof(cl_mem), &memoryObjects[2]));
    if (not setKernelArgumentsSuccess) {
        cleanUpOpenCL();
        std::cerr << "Failed setting OpenCL kernel arguments. " << __FILE__ 
                  << ":"<< __LINE__ << std::endl;
        exit(1);
    }

    /* Determine the work-group size and index space for the kernel */
    const size_t globalWorkSize[1] = {arraySize};
    const size_t localWorkSize[1] = { 1 };

    /* Enqueue the kernel for execution in the command queue */
    //for (int j = 0; j < ITER; j++) {
        if (not checkSuccess(clEnqueueNDRangeKernel(commandQueue, kernel, 1, 
                NULL, globalWorkSize, localWorkSize, 0, NULL, NULL))) {
            
            cleanUpOpenCL();
            std::cerr << "Failed enqueuing the kernel. " << __FILE__ << ":" 
                      << __LINE__ <<std::endl;
            exit(1);
        }
    //}

    /* Get a pointer to the output data */
    output = (double*)clEnqueueMapBuffer(commandQueue, 
                    memoryObjects[2], CL_TRUE, CL_MAP_READ, 0, 
                    arraySize, 0, NULL, NULL, &errorNumber);

    if (not checkSuccess(errorNumber)) {

        cleanUpOpenCL();
        std::cerr << "Failed to map buffer " << __FILE__ << ":"
                  << __LINE__ << std::endl;
        exit(1); 
    }

    /* Wait for kernel execution */
    if (not checkSuccess(clFinish(commandQueue))) {

        cleanUpOpenCL();
        std::cerr << "Failed waiting for kernel execution to finish. "
                  << __FILE__ << ":"<< __LINE__ << std::endl;
        exit(1);
    }


    /* Unmap the memory objects as we finished using them in the CPU */
    if (not checkSuccess(clReleaseMemObject(memoryObjects[0]))) {

        cleanUpOpenCL();
        std::cerr << "Unmapping memory objects failed " << __FILE__ << ":"
                  << __LINE__ << std::endl;
        exit(1);
    }
    if (not checkSuccess(clReleaseMemObject(memoryObjects[1]))) {

        cleanUpOpenCL();
        std::cerr << "Unmapping memory objects failed " << __FILE__ << ":"
                  << __LINE__ << std::endl;
        exit(1);
    }
    if (not checkSuccess(clEnqueueUnmapMemObject(commandQueue, 
                    memoryObjects[2], output, 0, NULL, NULL))) {

        cleanUpOpenCL();
        std::cerr << "Unmapping memory objects failed " << __FILE__ << ":"
                  << __LINE__ << std::endl;
        exit(1);
    }
}

int main (int argc, char* argv[]) {

    struct timespec start, end;
    
    initOpenCL();

    /* START Measurements */
    get_date(argc, argv);
    clock_gettime(CLOCK_MONOTONIC, &start);

    int arraySize = SIZE;
    size_t bufferSize = arraySize * sizeof(double);
    double* inputA = (double*) malloc (bufferSize);
    double* inputB = (double*) malloc (bufferSize);
    double* output = (double*) malloc (bufferSize);


    /* Initilise data */
    initialize_data(arraySize, inputA, inputB);

    /* Computation */
    vector_sum(arraySize, inputA, inputB, output);

    /* END Measurements */
    clock_gettime(CLOCK_MONOTONIC, &end);
    get_date(argc, argv);

    /* Check results */
    //check_results(arraySize, inputA, inputB, output);

    /* Cleaning */
    cleanUpOpenCL();

    double res=0;
    int i;
    for (i = 0; i < arraySize; i++) {
      res = res + output[i] ;
    }

    times = (((double)(end.tv_sec  - start.tv_sec)*1000) +
             ((double)(end.tv_nsec - start.tv_nsec)/1000000));
    //cout << "Time to finish: " << times << " ms" << endl;
    printf("Time to finish %6.0f ms [check = %e]\n", times, res);
}


