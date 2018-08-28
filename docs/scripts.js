function setFocus() {
    $("#player_input_field").focus();
}

$("#player_input_form").submit(function() {
    $("#player_input_form").trigger("reset");
    // do more stuff later
    return false;
});
