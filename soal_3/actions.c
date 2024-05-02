#include <stdio.h>
#include <string.h>

char *selisih(float jarak) {
    if (jarak < 3.5) {
        return "Gogogo";
    } else if (jarak >= 3.5 && jarak <= 10) {
        return "Push";
    } else {
        return "Stay out of trouble";
    }
}

char *bensin(float persenan) {
    if (persenan > 80) {
        return "Push Push Push";
    } else if (persenan >= 50 && persenan <= 80) {
        return "You can go";
    } else {
        return "Conserve Fuel";
    }
}

char *ban(int sisa) {
    if (sisa > 80) {
        return "Go Push Go Push";
    } else if (sisa >= 50 && sisa <= 80) {
        return "Good Tire Wear";
    } else if (sisa >= 30 && sisa < 50) {
        return "Conserve Your Tire";
    } else {
        return "Box Box Box";
    }
}

char *gantiBan(char *type) {
    if (strcmp(type, "Soft") == 0) {
        return "Mediums Ready";
    } else if (strcmp(type, "Medium") == 0) {
        return "Box for Softs";
    } else {
        return "Invalid";
    }
}
