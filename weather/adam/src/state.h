typedef enum _state
{
    INIT,
    WELCOME,
    TIME,
    OPTIONS,
    LOCATION,
    WEATHER,
    FORECAST,
    DONE
} State;

extern State state;