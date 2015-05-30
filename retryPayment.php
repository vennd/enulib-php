<?php
/**
 * Created by IntelliJ IDEA.
    * User: Jeremy
    * Date: 20/05/2015
    * Time: 1:46 AM
    */

// Requires the following extensions to be uncommented from php.ini:
// extension=php_curl.dll
// extension=php_openssl.dll

include "enulib.inc";

// The id of the payment to attempt to retry to process.
// The payment must be in a 'manual' or an 'error' state to retry.
// The retryPayment API call can only be called by the same API key that was used to create the payment in the first place.
$payment_id = '487055f45d9fe0528dcb';

$enulib = new enuLib();
$result = $enulib->retryPayment($payment_id, $return_status);

if ($return_status == 0) {
    printf("Payment retry for payment id %s successful.", $payment_id);
}
else {
    printf("Payment retry failed for payment id %s.\n", $payment_id);
    printf("Error code: %d, error description: %s.\n", $result['code'], $result['description']);
}

?>