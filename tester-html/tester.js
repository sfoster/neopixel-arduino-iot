"use strict";

var stopNow;
const NUM_PIXELS = 60;

let gNowMs, gPreviousMs;
let stateTransitionTime = 0;
let animations = {};
let fxController;
let inputController;
let inputState = {};

const StateEnum = {
  start_state: 0,
  off_transition: 1,
  off_state: 2,
  connecting: 3,
  connected: 4,
};
let currentState;
let previousState;

// ================================
// Helpers, shims

function clamp(num, lbound, ubound) {
  if (num < lbound) {
    num = lbound;
  }
  if (num > ubound) {
    num = ubound;
  }
  return num;
}

function debugPrint(...args) {
  console.log(...args);
}

function millis() {
  return Date.now();
}
// ================================

function setup() {
  gNowMs = millis();
  gPreviousMs = gNowMs;

  currentState = StateEnum.start_state;

  fxController = new Fx_Controller(NUM_PIXELS);
  fxController.start();

  inputController = new Input_Controller(inputState);
  inputController.start();
  display();
}

function processInput() {
  // say we got a remote command saying "gamestart"
  // turn that into something actionable in updateState
  if (currentState !== StateEnum.connected) {
    return;
  }
  if (!inputState.topic) {
    return;
  }

  switch (inputState.topic) {
    case "gamestart": {
      let params = new Fx_AnimationParams({r:0,g:0,b:0}, {r:0,g:155,b:255});
      let seq = new Fx_Controller_Clip(
        colorBlink, // animation
        gNowMs,                // startTime
        2000,                  // duration
        0,                     // repeat
        params,
      );
      fxController.addClip(seq);
      break;
    }
    case "gameend": {
      let params = new Fx_AnimationParams({r:0,g:155,b:255}, {r:255,g:255,b:0});
      let seq = new Fx_Controller_Clip(
        colorBlink, // animation
        gNowMs,                // startTime
        2000,                  // duration
        0,                     // repeat
        params,
      );
      fxController.addClip(seq);
      break;
    }
    // reset, restart, flush, startover, starting
    case "starting": {
      fxController.reset();

      break;
    }
    case "playanim": {
      let animName = inputState.params.animationName;
      if (typeof window[animName] !== "function") {
        console.log("Handling topic playanim, unknown animation: " + animName, typeof window[animName]);
        return;
      }
      // convert to numbers treating input as hex
      let startColorHex = parseInt(inputState.params.startColor.substring(1), 16);
      let endColorHex = parseInt(inputState.params.endColor.substring(1), 16);
      // shift over and apply mask to get the bits we need (value of 0-255)
      let startColor = {
        r: startColorHex >> 16 & 0xff,
        g: startColorHex >> 8 & 0xff,
        b: startColorHex >> 0 & 0xff,
      };
      let endColor = {
        r: endColorHex >> 16 & 0xff,
        g: endColorHex >> 8 & 0xff,
        b: endColorHex >> 0 & 0xff,
      };

      let duration = parseInt(inputState.params.duration);
      let repeat = parseInt(inputState.params.repeat || 0);

      let params = new Fx_AnimationParams(startColor, endColor,
                                          inputState.params.initialDirection,
                                          inputState.params.duration);
      console.log("playanim got params: ", params);
      let seq = new Fx_Controller_Clip(
        window[animName], // animation
        gNowMs,                // startTime
        duration,                  // duration
        repeat,                     // repeat
        params,
      );
      fxController.addClip(seq);
      break;
    }
    default:
      console.log("Unknown topic: " + inputState.topic);
      break;
  }
  // clear state data once we've handled it
  delete inputState.topic;
  delete inputState.params;
}

function loop() {
  gNowMs = millis();
  // inputController.update();
  processInput();
  updateState();
  fxController.updateTimeline();
  display();
  if (!stopNow) {
    window.requestAnimationFrame(loop);
  }
}

function updateState() {

  // states are predefined macro-level states of being for the whole thing
  // like "starting", "connecting", "connected", "playing", "disconnected"
  // the may utilize the animation engine, or not

  // console.log("updateState, currentState: " + currentState);
  switch (currentState) {
    case StateEnum.start_state :
      // do something??
      stateTransitionTime = 0;
      previousState = currentState;
      currentState = StateEnum.connecting;
      console.log("updateState, switching to 'connecting' state");
      break;
    case StateEnum.connecting :
      if (previousState !== currentState) {
        console.log("updateState, connecting state entry");
        // state entry, queue up an animation
        let params = new Fx_AnimationParams({r:0,g:155,b:255}, {r:255,g:255,b:0});
        let clip = new Fx_Controller_Clip(
          colorBlink, // animation
          gNowMs,                  // startTime
          2000,                    // duration
          Infinity,                // repeat
          params,
        );
        fxController.addClip(clip);

        stateTransitionTime = gNowMs + 2000;
        previousState = currentState;
      } else if (inputState.connected) {
        stateTransitionTime = 0;
        previousState = currentState;
        currentState = StateEnum.connected;
      } else if (gNowMs >= stateTransitionTime) {
        // continue trying to connect? or do we eventually reset
        // console.log("still connecting...");
        stateTransitionTime = gNowMs + 2000;
      }
      break;
    case StateEnum.off_transition :
      if (previousState !== currentState) {
        // state entry
        stateTransitionTime = gNowMs + 2000;

        let params = new Fx_AnimationParams();
        let clip = new Fx_Controller_Clip(
          fadeToBlack, // animation
          gNowMs,                    // startTime
          2000,                      // duration
          0,                         // repeat
          params,
        );
        fxController.addClip(clip);
      } else if (gNowMs >= stateTransitionTime) {
        previousState = currentState;
        currentState = StateEnum.off_state;
      }
      break;
    case StateEnum.off_state :
      fxController.reset();
      // inputController.reset();
      break;
    case StateEnum.connected :
      // kind of an "idle" or "getting input" state
      if (previousState !== currentState) {
        // state entry, queue up an animation
        fxController.reset();
        let params = new Fx_AnimationParams();
        let clip = new Fx_Controller_Clip(
          bounce,      // animation
          gNowMs,                    // startTime
          2000,                      // duration
          Infinity,                  // repeat
          params,
        );
        fxController.addClip(clip);
        previousState = currentState;
      }
      if (!inputState.connected) {
        previousState = currentState;
        currentState = StateEnum.connecting;
        return;
      }
      break;
  }
}

function display() {
  for(let i=0; i<NUM_PIXELS; i++) {
    let r = clamp(
      fxController.backgroundPixels[i].r + fxController.foregroundPixels[i].r,
      0, 255
    );
    let g = clamp(
      fxController.backgroundPixels[i].g + fxController.foregroundPixels[i].g,
      0, 255
    );
    let b = clamp(
      fxController.backgroundPixels[i].b + fxController.foregroundPixels[i].b,
      0, 255
    );
    setPixelColor(i, r, g, b);
  }
}

function main() {
  currentState = StateEnum.start_state;
  console.log("preparing strip");
  prepareControls();
  prepareStrip();
  console.log("calling setup");
  setup();
  loop();
  console.log("/main");
}

function prepareStrip() {
  let container = document.querySelector("#strip");
  let scale = 300;
  for(let i=0; i<NUM_PIXELS; i++) {
    let child = document.createElement("div");
    child.classList.add("led");
    child.id = "led_" + (NUM_PIXELS -1 - i);
    var x = scale * Math.cos(Math.PI * i / NUM_PIXELS);
    var y = scale * Math.sin(-1 * Math.PI * (i + 0.5) / NUM_PIXELS);
    child.style.left = `${x}px`;
    child.style.top = `${y}px`;
    container.appendChild(child);
  }
}

function prepareControls() {
  let select = document.querySelector("#anim-list");
  let animations = [
    "colorBlink",
    "endWithColor",
    "bounce",
    "allOff"
  ];
  for (let name of animations) {
    let option = document.createElement("option");
    option.value = option.textContent = name;
    select.appendChild(option);
  }
  let presetsContainer = document.querySelector("#presets");
  for (let [name, params] of Object.entries(presets)) {
    let item = document.createElement("li");
    let button = document.createElement("input");
    button.type = "button";
    button.textContent = button.value = name;
    item.appendChild(button);
    item.addEventListener("click", () => {
      theForm.setFormValues(params);
    });
    presetsContainer.appendChild(item);
  }

  console.log("controls populated");
  requestAnimationFrame(() => {
    console.log("rAF, calling form.init()");
    theForm.init();
  });
}
function setPixelColor(i, r, g, b) {
  let led = document.getElementById("led_" + i);
  led.style.backgroundColor = `rgb(${r}, ${g}, ${b})`;
}

document.addEventListener("DOMContentLoaded", main);

