#include ".\include\kiss_fftr.h"
#include ".\include\raylib.h"
#include <stdio.h>

#define CIRCULAR_BUFFER(TYPE, SIZE, NAME)\
    typedef struct{\
        int Index;\
        TYPE Buffer[SIZE];\
    }NAME;

#define CIRCULAR_BUFFER_ASIGN(BUF_TYPE, DATA_TYPE, SIZE)\
    void BUF_TYPE##_assign(BUF_TYPE* s, DATA_TYPE data){\
        if(s == NULL)\
            return;\
        \
        if(s->Index > SIZE)\
            s->Index = 0;\
        \
        s->Buffer[s->Index] = data;\
        s->Index += 1;\
    }

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 425

#define NFFT 48000
#define NYQ_FACTOR  4.0
#define SIG_RATE    0.0001
#define SIG_FREQ (1.0 / SIG_RATE)
#define SAMP_RATE (SIG_RATE / NYQ_FACTOR)
#define SAMP_FREQ (1.0 / SAMP_RATE)

CIRCULAR_BUFFER(kiss_fft_scalar, NFFT, samples_t)
CIRCULAR_BUFFER_ASIGN(samples_t, kiss_fft_scalar, NFFT)

int main(void){

    //Gathering Sample Data
    samples_t data;
    for(int i = 0; i < NFFT; i++){
        samples_t_assign(&data, sin(2.0*PI*SIG_FREQ*i*SAMP_RATE));
    }

    // //Calculate the FFT of The Sample Data
    // kiss_fftr_cfg cfg = kiss_fftr_alloc(NFFT, 0, NULL, NULL);
    // kiss_fft_cpx fft[NFFT/2 + 1];

    // kiss_fftr(cfg, data, fft);

    // //STORE OUR CALCULATED FFT
    // FILE* file = fopen("fft.txt", "w");
    // for(int i = 0; i < NFFT/2+1; i++){
    //     double abs = sqrt((fft[i].r*fft[i].r)+(fft[i].i*fft[i].i)) / (NFFT/2+1);
    //     double freq = i * (SAMP_FREQ / (NFFT));
    //     fprintf(file, "[i=%d -> f=%.3lf] - (%.3f, %.3f) -> abs = %.7lf\n", i, freq, fft[i].r, fft[i].i, abs);
    // }

    // fclose(file);

    // kiss_fft_free(cfg);


    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Random Walker FFT");
    SetTargetFPS(60);

    double t = GetTime() - GetFrameTime();
    kiss_fftr_cfg cfg = kiss_fftr_alloc(NFFT, 0, NULL, NULL);
    kiss_fft_cpx fft[NFFT/2 + 1];

    while(!WindowShouldClose()){

        //  UPDATE SAMPLES AND CALCULATE FFT
        samples_t_assign(&data, sin(2.0*PI*SIG_FREQ*t) * (GetRandomValue(-10, 10) / 10.0));
        kiss_fftr(cfg, data.Buffer, fft);

        BeginDrawing();
            ClearBackground(LIGHTGRAY);
            //  GRAPHICS TODO
        EndDrawing();

        //UPDATE DATA TIME INCREMENT
        t = (GetTime() - GetFrameTime()) * SAMP_RATE;
    }
    kiss_fft_free(cfg);
    CloseWindow();

    //STORE OUR CALCULATED FFT
    FILE* file = fopen("fft.txt", "w");
    for(int i = 0; i < NFFT/2+1; i++){
        double abs = sqrt((fft[i].r*fft[i].r)+(fft[i].i*fft[i].i)) / (NFFT/2+1);
        double freq = i * (SAMP_FREQ / (NFFT));
        fprintf(file, "[i=%d -> f=%.3lf] - (%.3f, %.3f) -> abs = %.7lf\n", i, freq, fft[i].r, fft[i].i, abs);
    }

    fclose(file);
}