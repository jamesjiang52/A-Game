function setFocusToInputField() {
    $("#player_input_field").focus();
}

function scrollOutputToBottom() {
    var outputContainer = $(".output-container");
    outputContainer.animate({scrollTop: outputContainer.get(0).scrollHeight});
}

$("#player_input_form").submit(function() {
    // right now, just have it append to the output field
    $("#output").append($("#player_input_field").val() + "<br><br>");
    
    $("#player_input_form").trigger("reset");
    scrollOutputToBottom();
    
    return false;
});
