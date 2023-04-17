type Tunits = (metric, imperial, unknown);

type TOptions = record
        apiKeyOW: string[32];
        refreshInterval: byte;
        units: Tunits;
        showRegion: boolean;
        detectLocation: boolean;
        theme: string[12];
        maxPrecision: byte;
        unused: array [0..10] of byte;
end;        

type TLocation = record
        city: string[40];
        region_code: string[2];
        country_code: string[2];
        latitude: string[7];
        longitude: string[7]; 
end;

type TTheme = record
    name: string[20];
    author: string[20];
    gfx: string[12];
    font: string[12];
    colorsNTSC: array [0..PALLETE_MAX] of byte;
    colorsPAL: array [0..PALLETE_MAX] of byte;
end;
