<?php
$generosity = $_GET['generosity'];
switch ($generosity) {
    case 'low' : break;
    case 'Low' : break;
    case 'LOW' : break;
    case 'medium' : break;
    case 'Medium' : break;
    case 'MEDIUM' : break;
    case 'high' : break;
    case 'High' : break;
    case 'HIGH' : break;
    default: $generosity = FALSE;
}
if (!$generosity) {
    $generosity = "Medium";
}
$donation = 'https://www.paypal.com/cgi-bin/webscr' .
        '?cmd=_s-xclick&hosted_button_id=CTTP93XJCV3DS' .
        '&on0=Generosity&os0=' . $generosity;
header('Location: ' . $donation);
?>