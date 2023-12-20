void playVideoWithMetadata(const char* videoFilePath) {
    // Initialize FFmpeg if needed

    // Open the video file
    AVFormatContext* formatContext = nullptr;
    if (avformat_open_input(&formatContext, videoFilePath, nullptr, nullptr) != 0) {
        std::cerr << "Error: Could not open video file." << std::endl;
        return;
    }

    if (avformat_find_stream_info(formatContext, nullptr) < 0) {
        std::cerr << "Error: Could not find stream information." << std::endl;
        avformat_close_input(&formatContext);
        return;
    }

    // Find the video stream
    int videoStreamIndex = -1;
    for (unsigned int i = 0; i < formatContext->nb_streams; i++) {
        if (formatContext->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
            videoStreamIndex = i;
            break;
        }
    }

    if (videoStreamIndex == -1) {
        std::cerr << "Error: Could not find video stream in the file." << std::endl;
        avformat_close_input(&formatContext);
        return;
    }

    AVCodecParameters* codecParameters = formatContext->streams[videoStreamIndex]->codecpar;

    // Get the video framerate from metadata
    AVRational frameRate = formatContext->streams[videoStreamIndex]->avg_frame_rate;
    double fps = av_q2d(frameRate);

    // Adjust the framerate if necessary (e.g., change to 30 fps)
    if (fps != 30.0) {
        fps = 30.0; // Change to the desired framerate
        frameRate = av_d2q(fps, 1000);
        formatContext->streams[videoStreamIndex]->r_frame_rate = frameRate;
    }
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "Error: SDL initialization failed." << std::endl;
        return;
    }


    // Open the video file
    formatContext = nullptr;
    if (avformat_open_input(&formatContext, videoFilePath, nullptr, nullptr) != 0) {
        std::cerr << "Error: Could not open video file." << std::endl;
        return;
    }

    // Retrieve video stream information
    if (avformat_find_stream_info(formatContext, nullptr) < 0) {
        std::cerr << "Error: Could not find stream information." << std::endl;
        avformat_close_input(&formatContext);
        return;
    }

    // Find the video stream
    videoStreamIndex = -1;
    for (unsigned int i = 0; i < formatContext->nb_streams; i++) {
        if (formatContext->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
            videoStreamIndex = i;
            break;
        }
    }

    if (videoStreamIndex == -1) {
        std::cerr << "Error: Could not find video stream in the file." << std::endl;
        avformat_close_input(&formatContext);
        return;
    }

    codecParameters = formatContext->streams[videoStreamIndex]->codecpar;

    // Create an SDL window with the video resolution
    SDL_Window* window = SDL_CreateWindow("Video Player", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                          codecParameters->width, codecParameters->height, SDL_WINDOW_SHOWN);

    if (!window) {
        std::cerr << "Error: SDL window creation failed." << std::endl;
        avformat_close_input(&formatContext);
        return;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "Error: SDL renderer creation failed." << std::endl;
        SDL_DestroyWindow(window);
        avformat_close_input(&formatContext);
        return;
    }

    SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_YV12, SDL_TEXTUREACCESS_STREAMING,
                                             codecParameters->width, codecParameters->height);

    if (!texture) {
        std::cerr << "Error: SDL texture creation failed." << std::endl;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        avformat_close_input(&formatContext);
        return;
    }

    // Create a video frame and packet
    AVFrame* frame = av_frame_alloc();
    AVPacket packet;

    // Initialize the video scaler
    struct SwsContext* swsContext = sws_getContext(codecParameters->width, codecParameters->height,
                                                   AV_PIX_FMT_YUV420P, codecParameters->width,
                                                   codecParameters->height, AV_PIX_FMT_YUV420P,
                                                   SWS_BILINEAR, nullptr, nullptr, nullptr);

    // Read and display video frames
    while (av_read_frame(formatContext, &packet) >= 0) {
        if (packet.stream_index == videoStreamIndex) {
            avcodec_send_packet(nullptr, &packet);
            avcodec_receive_frame(nullptr, frame);

            sws_scale(swsContext, frame->data, frame->linesize, 0, codecParameters->height,
                      frame->data, frame->linesize);

            SDL_UpdateYUVTexture(texture, nullptr, frame->data[0], frame->linesize[0],
                                  frame->data[1], frame->linesize[1], frame->data[2], frame->linesize[2]);

            SDL_RenderClear(renderer);
            SDL_RenderCopy(renderer, texture, nullptr, nullptr);
            SDL_RenderPresent(renderer);
        }

        av_packet_unref(&packet);
    }

    // Clean up resources
    sws_freeContext(swsContext);
    av_frame_free(&frame);
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    avformat_close_input(&formatContext);
    SDL_Quit();
}
