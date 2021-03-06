#ifdef NANOGUI_PYTHON

#include "python.h"

void register_constants(py::module &m) {
    /* GLFW constants */
    {
        #define C(name) g.attr(#name) = py::int_(GLFW_##name);
        py::module g = m.def_submodule("glfw");
        C(KEY_UNKNOWN); C(KEY_SPACE); C(KEY_APOSTROPHE); C(KEY_COMMA);
        C(KEY_MINUS); C(KEY_PERIOD); C(KEY_SLASH); C(KEY_0); C(KEY_1);
        C(KEY_2); C(KEY_3); C(KEY_4); C(KEY_5); C(KEY_6); C(KEY_7); C(KEY_8);
        C(KEY_9); C(KEY_SEMICOLON); C(KEY_EQUAL); C(KEY_A); C(KEY_B); C(KEY_C);
        C(KEY_D); C(KEY_E); C(KEY_F); C(KEY_G); C(KEY_H); C(KEY_I); C(KEY_J);
        C(KEY_K); C(KEY_L); C(KEY_M); C(KEY_N); C(KEY_O); C(KEY_P); C(KEY_Q);
        C(KEY_R); C(KEY_S); C(KEY_T); C(KEY_U); C(KEY_V); C(KEY_W); C(KEY_X);
        C(KEY_Y); C(KEY_Z); C(KEY_LEFT_BRACKET); C(KEY_BACKSLASH);
        C(KEY_RIGHT_BRACKET); C(KEY_GRAVE_ACCENT); C(KEY_WORLD_1);
        C(KEY_WORLD_2); C(KEY_ESCAPE); C(KEY_ENTER); C(KEY_TAB);
        C(KEY_BACKSPACE); C(KEY_INSERT); C(KEY_DELETE); C(KEY_RIGHT);
        C(KEY_LEFT); C(KEY_DOWN); C(KEY_UP); C(KEY_PAGE_UP); C(KEY_PAGE_DOWN);
        C(KEY_HOME); C(KEY_END); C(KEY_CAPS_LOCK); C(KEY_SCROLL_LOCK);
        C(KEY_NUM_LOCK); C(KEY_PRINT_SCREEN); C(KEY_PAUSE); C(KEY_F1);
        C(KEY_F2); C(KEY_F3); C(KEY_F4); C(KEY_F5); C(KEY_F6); C(KEY_F7);
        C(KEY_F8); C(KEY_F9); C(KEY_F10); C(KEY_F11); C(KEY_F12); C(KEY_F13);
        C(KEY_F14); C(KEY_F15); C(KEY_F16); C(KEY_F17); C(KEY_F18); C(KEY_F19);
        C(KEY_F20); C(KEY_F21); C(KEY_F22); C(KEY_F23); C(KEY_F24); C(KEY_F25);
        C(KEY_KP_0); C(KEY_KP_1); C(KEY_KP_2); C(KEY_KP_3); C(KEY_KP_4);
        C(KEY_KP_5); C(KEY_KP_6); C(KEY_KP_7); C(KEY_KP_8); C(KEY_KP_9);
        C(KEY_KP_DECIMAL); C(KEY_KP_DIVIDE); C(KEY_KP_MULTIPLY);
        C(KEY_KP_SUBTRACT); C(KEY_KP_ADD); C(KEY_KP_ENTER); C(KEY_KP_EQUAL);
        C(KEY_LEFT_SHIFT); C(KEY_LEFT_CONTROL); C(KEY_LEFT_ALT);
        C(KEY_LEFT_SUPER); C(KEY_RIGHT_SHIFT); C(KEY_RIGHT_CONTROL);
        C(KEY_RIGHT_ALT); C(KEY_RIGHT_SUPER); C(KEY_MENU); C(KEY_LAST);
        C(MOD_SHIFT); C(MOD_CONTROL); C(MOD_ALT); C(MOD_SUPER);
        C(MOUSE_BUTTON_1); C(MOUSE_BUTTON_2); C(MOUSE_BUTTON_3);
        C(MOUSE_BUTTON_4); C(MOUSE_BUTTON_5); C(MOUSE_BUTTON_6);
        C(MOUSE_BUTTON_7); C(MOUSE_BUTTON_8); C(MOUSE_BUTTON_LAST);
        C(MOUSE_BUTTON_LEFT); C(MOUSE_BUTTON_RIGHT); C(MOUSE_BUTTON_MIDDLE);
        C(RELEASE); C(PRESS); C(REPEAT);
        #undef C
    }

    /* Entypo constants */
    {
        #define C(name) g.attr("ICON_" #name) = py::int_(ENTYPO_ICON_##name);
        py::module g = m.def_submodule("entypo");
        C(PHONE); C(MOBILE); C(MOUSE); C(ADDRESS); C(MAIL); C(PAPER_PLANE);
        C(PENCIL); C(FEATHER); C(ATTACH); C(INBOX); C(REPLY); C(REPLY_ALL);
        C(FORWARD); C(USER); C(USERS); C(ADD_USER); C(VCARD); C(EXPORT);
        C(LOCATION); C(MAP); C(COMPASS); C(DIRECTION); C(HAIR_CROSS); C(SHARE);
        C(SHAREABLE); C(HEART); C(HEART_EMPTY); C(STAR); C(STAR_EMPTY);
        C(THUMBS_UP); C(THUMBS_DOWN); C(CHAT); C(COMMENT); C(QUOTE); C(HOME);
        C(POPUP); C(SEARCH); C(FLASHLIGHT); C(PRINT); C(BELL); C(LINK);
        C(FLAG); C(COG); C(TOOLS); C(TROPHY); C(TAG); C(CAMERA); C(MEGAPHONE);
        C(MOON); C(PALETTE); C(LEAF); C(NOTE); C(BEAMED_NOTE); C(NEW);
        C(GRADUATION_CAP); C(BOOK); C(NEWSPAPER); C(BAG); C(AIRPLANE);
        C(LIFEBUOY); C(EYE); C(CLOCK); C(MIC); C(CALENDAR); C(FLASH);
        C(THUNDER_CLOUD); C(DROPLET); C(CD); C(BRIEFCASE); C(AIR);
        C(HOURGLASS); C(GAUGE); C(LANGUAGE); C(NETWORK); C(KEY); C(BATTERY);
        C(BUCKET); C(MAGNET); C(DRIVE); C(CUP); C(ROCKET); C(BRUSH);
        C(SUITCASE); C(TRAFFIC_CONE); C(GLOBE); C(KEYBOARD); C(BROWSER);
        C(PUBLISH); C(PROGRESS_3); C(PROGRESS_2); C(PROGRESS_1); C(PROGRESS_0);
        C(LIGHT_DOWN); C(LIGHT_UP); C(ADJUST); C(CODE); C(MONITOR);
        C(INFINITY); C(LIGHT_BULB); C(CREDIT_CARD); C(DATABASE); C(VOICEMAIL);
        C(CLIPBOARD); C(CART); C(BOX); C(TICKET); C(RSS); C(SIGNAL);
        C(THERMOMETER); C(WATER); C(SWEDEN); C(LINE_GRAPH); C(PIE_CHART);
        C(BAR_GRAPH); C(AREA_GRAPH); C(LOCK); C(LOCK_OPEN); C(LOGOUT);
        C(LOGIN); C(CHECK); C(CROSS); C(SQUARED_MINUS); C(SQUARED_PLUS);
        C(SQUARED_CROSS); C(CIRCLED_MINUS); C(CIRCLED_PLUS); C(CIRCLED_CROSS);
        C(MINUS); C(PLUS); C(ERASE); C(BLOCK); C(INFO); C(CIRCLED_INFO);
        C(HELP); C(CIRCLED_HELP); C(WARNING); C(CYCLE); C(CW); C(CCW);
        C(SHUFFLE); C(BACK); C(LEVEL_DOWN); C(RETWEET); C(LOOP);
        C(BACK_IN_TIME); C(LEVEL_UP); C(SWITCH); C(NUMBERED_LIST);
        C(ADD_TO_LIST); C(LAYOUT); C(LIST); C(TEXT_DOC); C(TEXT_DOC_INVERTED);
        C(DOC); C(DOCS); C(LANDSCAPE_DOC); C(PICTURE); C(VIDEO); C(MUSIC);
        C(FOLDER); C(ARCHIVE); C(TRASH); C(UPLOAD); C(DOWNLOAD); C(SAVE);
        C(INSTALL); C(CLOUD); C(UPLOAD_CLOUD); C(BOOKMARK); C(BOOKMARKS);
        C(OPEN_BOOK); C(PLAY); C(PAUS); C(RECORD); C(STOP); C(FF); C(FB);
        C(TO_START); C(TO_END); C(RESIZE_FULL); C(RESIZE_SMALL); C(VOLUME);
        C(SOUND); C(MUTE); C(FLOW_CASCADE); C(FLOW_BRANCH); C(FLOW_TREE);
        C(FLOW_LINE); C(FLOW_PARALLEL); C(LEFT_BOLD); C(DOWN_BOLD); C(UP_BOLD);
        C(RIGHT_BOLD); C(LEFT); C(DOWN); C(UP); C(RIGHT); C(CIRCLED_LEFT);
        C(CIRCLED_DOWN); C(CIRCLED_UP); C(CIRCLED_RIGHT); C(TRIANGLE_LEFT);
        C(TRIANGLE_DOWN); C(TRIANGLE_UP); C(TRIANGLE_RIGHT); C(CHEVRON_LEFT);
        C(CHEVRON_DOWN); C(CHEVRON_UP); C(CHEVRON_RIGHT);
        C(CHEVRON_SMALL_LEFT); C(CHEVRON_SMALL_DOWN); C(CHEVRON_SMALL_UP);
        C(CHEVRON_SMALL_RIGHT); C(CHEVRON_THIN_LEFT); C(CHEVRON_THIN_DOWN);
        C(CHEVRON_THIN_UP); C(CHEVRON_THIN_RIGHT); C(LEFT_THIN); C(DOWN_THIN);
        C(UP_THIN); C(RIGHT_THIN); C(ARROW_COMBO); C(THREE_DOTS); C(TWO_DOTS);
        C(DOT); C(CC); C(CC_BY); C(CC_NC); C(CC_NC_EU); C(CC_NC_JP); C(CC_SA);
        C(CC_ND); C(CC_PD); C(CC_ZERO); C(CC_SHARE); C(CC_REMIX); C(DB_LOGO);
        C(DB_SHAPE); C(GITHUB); C(C_GITHUB); C(FLICKR); C(C_FLICKR); C(VIMEO);
        C(C_VIMEO); C(TWITTER); C(C_TWITTER); C(FACEBOOK); C(C_FACEBOOK);
        C(S_FACEBOOK); C(GOOGLEPLUS); C(C_GOOGLEPLUS); C(PINTEREST);
        C(C_PINTEREST); C(TUMBLR); C(C_TUMBLR); C(LINKEDIN); C(C_LINKEDIN);
        C(DRIBBBLE); C(C_DRIBBBLE); C(STUMBLEUPON); C(C_STUMBLEUPON);
        C(LASTFM); C(C_LASTFM); C(RDIO); C(C_RDIO); C(SPOTIFY); C(C_SPOTIFY);
        C(QQ); C(INSTAGRAM); C(DROPBOX); C(EVERNOTE); C(FLATTR); C(SKYPE);
        C(C_SKYPE); C(RENREN); C(SINA_WEIBO); C(PAYPAL); C(PICASA);
        C(SOUNDCLOUD); C(MIXI); C(BEHANCE); C(GOOGLE_CIRCLES); C(VK);
        C(SMASHING);
        #undef C
    }
}

#endif
