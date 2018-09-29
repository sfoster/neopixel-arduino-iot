function Input_Controller(state = {}) {
  this.state = state;
}

Input_Controller.prototype.start = function() {
  // mock being connected after 5s
  setTimeout(() => {
    console.log("Mock connected event, setting state.connected true");
    this.state.connected = true;
  }, 1000);
}

Input_Controller.prototype.handleRequest = function(topic, params) {
  // maybe its an mqtt handler, or a http get handler
  this.state.topic = topic;
  this.state.params = params;
}
