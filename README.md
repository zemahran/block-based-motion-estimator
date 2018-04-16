### Description

This is a backward prediction block-based motion estimator utilizing the spiral full search algorithm.
The testing was done on two 288 × 352 luminance frames adopted from [The Akiyo News Broadcast](https://media.xiph.org/video/derf/).
The second frame is used to generate a prediction (residual frame and motion vectors) for the first one.

### Dependencies

* [OpenCV](https://www.youtube.com/watch?v=M-VHaLHC4XI&t=1s)

### Testing

1. Clone the repo or download the `.zip` file.
> https://github.com/zemahran/block-based-motion-estimator.git
2. In Visual Studio, open the project solution named "Project I".
3. Build and run the "Source.cpp" file.
