// Clay Config: see https://github.com/pebble/clay
module.exports = [
  {
    "type": "heading",
    "defaultValue": "Settings"
  },
  {
  "type": "heading",
  "defaultValue": "Weather will not show unless a weather provider is selected and set up.  Set complications to the same colour as the background to hide them.",
  "size":6
  },
  {
    "type": "section",
    "items": [
      {
       "type": "select",
       "messageKey": "FontChoice",
       "defaultValue": 'steel',
       "label": "Time & Date Font",
       "capabilities":["HEALTH"],
       "options": [
         {
           "label": "Steelfish",
           "value": "steel",
         },
         {
           "label": "Open Dyslexic",
           "value": "opend",
         },
         {
           "label": "Copse",
           "value": "cop",
         }
       ]
      },
      {
        "type": "toggle",
        "messageKey": "HealthOff",
        "label": "Switch off Steps",
        "defaultValue": true,
        "capabilities":["HEALTH"]
      },
      {
        "type": "heading",
        "defaultValue": "Top Section",
        "size":4
      },
      {
        "type": "color",
        "messageKey": "FrameColor2",
        "defaultValue": "0x000000",
        "label": "Background Colour",
        "allowGray":true
      },
      {
        "type": "color",
        "messageKey": "Text7Color",
        "defaultValue": "0xFFFFFF",
        "label": "Weather icon Colour",
        "allowGray":true
      },
      {
        "type": "color",
        "messageKey": "Text8Color",
        "defaultValue": "0xFFFFFF",
        "label": "Temperature Colour",
        "allowGray":true
      },
      {
        "type": "color",
        "messageKey": "Text9Color",
        "defaultValue": "0xFFFFFF",
        "label": "Wind Speed Colour",
        "allowGray":true
      },
      {
        "type": "color",
        "messageKey": "Text10Color",
        "defaultValue": "0xFFFFFF",
        "label": "Wind Direction Colour",
        "allowGray":true
      },
      {
        "type": "color",
        "messageKey": "Text6Color",
        "defaultValue": "0xFFFFFF",
        "label": "Battery Bar Colour",
        "allowGray":true
      },
      {
        "type": "heading",
        "defaultValue": "Middle Section",
        "size":4
      },
      {
        "type": "color",
        "messageKey": "FrameColor1",
        "defaultValue": "0x000000",
        "label": "Background Colour",
        "allowGray":true
      },
      {
        "type": "color",
        "messageKey": "HourColor",
        "defaultValue": "0xFFFFFF",
        "label": "Time Colour",
        "allowGray":true
      },
      {
        "type": "color",
        "messageKey": "MinColor",
        "defaultValue": "0xFFFFFF",
        "label": "Date Colour",
        "allowGray":true
      },
      {
        "type": "color",
        "messageKey": "Text3Color",
        "defaultValue": "0xFFFFFF",
        "label": "Day of the Week Colour",
        "allowGray":true
      },
      {
        "type": "heading",
        "defaultValue": "Bottom Section",
        "size":4
      },
      {
        "type": "color",
        "messageKey": "FrameColor",
        "defaultValue": "0x000000",
        "label": "Background Colour",
        "allowGray":true
      },
      {
        "type": "color",
        "messageKey": "Text1Color",
        "defaultValue": "0xFFFFFF",
        "label": "Steps Colour",
        "allowGray":true,
        "capabilities":["HEALTH"]
      },
      {
        "type": "color",
        "messageKey": "Text5Color",
        "defaultValue": "0xFFFFFF",
        "label": "BT & QT Colour",
        "allowGray":true
      },
       {
        "type": "color",
        "messageKey": "Text2Color",
        "defaultValue": "0xFFFFFF",
        "label": "Sunrise & Sunset Colour",
        "allowGray":true
      },
      {
        "type":"color",
        "messageKey":"Text4Color",
        "defaultValue":"0xFFFFFF",
        "label":"Moon Colour",
        "allowGray":true
      },
      {"type": "section",
       "items": [
         {
           "type": "heading",
           "defaultValue": "Night Theme",
           "size":4
         } ,
         {
           "type": "toggle",
           "messageKey": "NightTheme",
           "label": "Activate Night Theme",
           "defaultValue": false,
         },
         {
           "type": "heading",
           "defaultValue": "Top Section Night",
           "size":4
         },
         {
            "type": "color",
            "messageKey": "FrameColor2N",
            "defaultValue": "0xFFFFFF",
            "label": "Background Colour",
            "allowGray":true
         },
         {
           "type": "color",
           "messageKey": "Text7ColorN",
           "defaultValue": "0x000000",
           "label": "Weather icon Colour",
           "allowGray":true
         },
         {
           "type": "color",
           "messageKey": "Text8ColorN",
           "defaultValue": "0x000000",
           "label": "Temperature Colour",
           "allowGray":true
         },
         {
           "type": "color",
           "messageKey": "Text9ColorN",
           "defaultValue": "0x000000",
           "label": "Wind Speed Colour",
           "allowGray":true
         },
         {
           "type": "color",
           "messageKey": "Text10ColorN",
           "defaultValue": "0x000000",
           "label": "Wind Direction Colour",
           "allowGray":true
         },
         {
          "type": "color",
          "messageKey": "Text6ColorN",
          "defaultValue": "0x000000",
          "label": "Battery Bar Colour",
          "allowGray":true
         },
         {
           "type": "heading",
           "defaultValue": "Middle Section Night",
           "size":4
         },
         {
           "type": "color",
           "messageKey": "FrameColor1N",
           "defaultValue": "0xFFFFFF",
           "label": "Background Colour",
           "allowGray":true
         },
         {
           "type": "color",
           "messageKey": "HourColorN",
           "defaultValue": "0x000000",
          "label": "Time Colour",
          "allowGray":true
         },
         {
          "type": "color",
          "messageKey": "MinColorN",
          "defaultValue": "0x000000",
          "label": "Date Colour",
          "allowGray":true
         },
         {
           "type": "color",
           "messageKey": "Text3ColorN",
           "defaultValue": "0x000000",
           "label": "Day of the Week Colour",
           "allowGray":true
         },
         {
           "type": "heading",
           "defaultValue": "Bottom Section Night",
           "size":4
         },
         {
            "type": "color",
            "messageKey": "FrameColorN",
            "defaultValue": "0xFFFFFF",
            "label": "Background Colour",
            "allowGray":true
         },
         {
          "type": "color",
          "messageKey": "Text1ColorN",
          "defaultValue": "0x000000",
          "label": "Steps Colour",
          "allowGray":true,
          "capabilities":["HEALTH"]
         },
         {
          "type": "color",
          "messageKey": "Text5ColorN",
          "defaultValue": "0x000000",
          "label": "BT & QT Colour",
          "allowGray":true
         },
         {
          "type": "color",
          "messageKey": "Text2ColorN",
          "defaultValue": "0x000000",
          "label": "Sunrise & Sunset Colour",
          "allowGray":true
         },
         {
          "type":"color",
          "messageKey":"Text4ColorN",
          "defaultValue":"0x000000",
          "label":"Moon Colour",
          "allowGray":true
        }
       ]
         }
       ]
      },
      {
        "type": "section",
        "items": [
          {
            "type": "heading",
            "defaultValue": "Weather settings"
          },
          {
            "type": "toggle",
            "messageKey": "WeatherUnit",
            "label": "Temperature in Fahrenheit",
            "defaultValue": false,
          },
            {
            "type": "select",
            "messageKey": "WindUnit",
            "label": "Wind speed",
            "defaultValue": "knots",
               "options": [
              {
                "label": "knots",
                "value": "kts"
              },
              {
                "label": "miles per hour",
                "value": "mph"
              },
                 {
                "label": "metres per second",
                "value": "ms"
              },
              {
                "label": "kilometres per hour",
                "value": "kph"
              }
            ]
          },
           {
            "type": "select",
            "messageKey": "WeatherProv",
            "defaultValue": "owm",
            "label": "Weather Provider",
            "options": [
              {
                "label": "OpenWeatherMap",
                "value": "owm"
              },
              {
                "label": "DarkSky",
                "value": "ds"
              }
            ]
          },
          {
             "type": "input",
             "messageKey": "Lat",
             "defaultValue": "",
             "label": "Manual Location - Latitude",
             "attributes": {
             "placeholder": "eg: 51.4962"
             }
           },
           {
              "type": "input",
              "messageKey": "Long",
              "defaultValue": "",
              "label": "Manual Location - Longitude",
              "description": "Leave both blank to use GPS location for sunrise & sunset times and weather. You can use <a href =https://www.google.com/maps>Google Maps</a> or <a href =https://www.openstreetmap.org/>OpenStreetMap</a> to find latitude & longitude.",
              "attributes": {
                "placeholder": "eg: -0.0989"
              }
            },
         {
            "type": "input",
            "messageKey": "APIKEY_User",
            "defaultValue": "",
            "label": "API Key",
            "description": "If left blank, the watch will attempt to request an api from your pmkey.xyz.  If you don't have an api key, weather data will not be displayed. Existing DarkSky APIs will work, but you can no longer sign up for a new one. You can still register for a free personal API key for <a href =https://home.openweathermap.org/users/sign_up/>OpenWeatherMap here</a>.",
            "attributes": {
              "placeholder": "Paste your API Key here"
            }
          },
         {
            "type": "input",
            "messageKey": "EmailPMKEY",
            "defaultValue": "",
            "label": "pmkey.xyz User",
            "description": "pmkey.xyz is a free service for Pebble users that allows you to safely store all your API keys in a single place. Check it out and sign up at <a href=https://www.pmkey.xyz/>pmkey.xyz</a>.",
            "attributes": {
              "placeholder": "eg: jane.smith@pmkey.xyz",
              "type": "email"
            }
          },
          {
            "type": "input",
            "messageKey": "PINPMKEY",
            "defaultValue": "",
            "label": "pmkey.xyz PIN",
            "attributes": {
              "placeholder": "eg: 12345"
            }
          },
          {
            "type": "slider",
            "messageKey": "UpSlider",
            "defaultValue": 30,
            "label": "Update frequency (minutes)",
            "description": "More frequent requests will drain your phone battery more quickly",
            "min": 15,
            "max": 120,
            "step": 15},
        ]
          },
      {
          "type": "submit",
          "defaultValue":"SAVE"
          },
          {
          "type": "heading",
          "defaultValue": "version v1.0",
          "size":6
          },
          {
          "type": "heading",
          "defaultValue": "Made in the UK",
          "size":6
          }
        ];
