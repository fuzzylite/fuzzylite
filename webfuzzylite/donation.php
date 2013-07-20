<?php

$value = $_GET['value'];
if(array_key_exists('generosity', $_GET)){
	$value = $_GET['generosity'];
}

switch ($value) {
    case 'low' : break;
    case 'Low' : break;
    case 'LOW' : break;
    case 'medium' : break;
    case 'Medium' : break;
    case 'MEDIUM' : break;
    case 'high' : break;
    case 'High' : break;
    case 'HIGH' : break;
    default: $value = FALSE;
}

if (!$value) {
    $value = "Medium";
}
$donation = 'https://www.paypal.com/cgi-bin/webscr' .
        '?cmd=_s-xclick&hosted_button_id=CVGVAHQ5DRYHW' .
        '&on0=Donation&os0=' . $value;
header('Location: ' . $donation);
?>
