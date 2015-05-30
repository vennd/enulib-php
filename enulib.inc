<?php
/**
 * Based upon: https://github.com/marinu666/PHP-btce-api
 */
 
class enuLib {
    protected $test_mode = true;
    protected $enu_api_base_url = "https://enu.io";
    protected $enu_api_test_base_url = "http://localhost:8080";

    protected $api_key;
    protected $api_secret;
    protected $nonce;
    protected $RETRY_FLAG = false;

    public function __construct() {
        $config_string = file_get_contents("enu_key.json");
        $config_json = json_decode($config_string, true);

        $this->api_key = $config_json{'key'};
        $this->api_secret = $config_json{'secret'};
        $this->nonce = time();
    }

    // Returns the base URL when constructing API calls.
    // Test mode is only for internal use where vennd.io is running a local API server
    public function baseURL() {
        if($this->test_mode ==false) {
            return $this->enu_api_base_url;
        }
        else {
            return $this->enu_api_test_base_url;
        }
    }

    // Performs a POST HTTP against the enu API
    public function postEnuAPI($url, $post_data, &$return_status) {
        return $this->doEnuAPI($url, "POST", $post_data, $return_status);
    }

    // Performs a GET HTTP against the enu API
    public function getEnuAPI($url, &$return_status) {
        // Set POST data to be an empty array to perform a GET
        $post_data = array();

        return $this->doEnuAPI($url, "GET", $post_data, $return_status);
    }

    // Performs a POST or a GET HTTP against the enu API, as per $method.
    public function doEnuAPI($url, $method, $post_data, &$return_status)
    {
        // Generate nonce
        $nonce = time();

        // encode into json
        $post_data_json = json_encode($post_data);

        // Generate the keyed hash value to post
        $sign = hash_hmac("sha512", $post_data_json, $this->api_secret);

        // Add to the headers
        $headers = array(
            'accessKey: ' . $this->api_key,
            'signature: ' . $sign,
            'nonce: ' . $nonce
        );

        // Create a CURL Handler for use
        $ch = null;
        $ch = curl_init();
        curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);
        curl_setopt($ch, CURLOPT_URL, $url);
        if ($method == "POST") {
            curl_setopt($ch, CURLOPT_POSTFIELDS, $post_data_json);
        }
        curl_setopt($ch, CURLOPT_HTTPHEADER, $headers);
        curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, true);
        curl_setopt($ch, CURLOPT_SSL_VERIFYHOST, 2);
        curl_setopt($ch, CURLOPT_CAINFO, "comodo_bundle.pem");

        // Send API Request
        $res = curl_exec($ch);
        $http_code = curl_getinfo($ch, CURLINFO_HTTP_CODE);

        // Check for failure & Clean-up curl handler
        if ($res === false) {
            $e = curl_error($ch);
            curl_close($ch);

            $return_status = -1000;

            return array('code' => -1000,
                'description' => $e);
        } else {
            curl_close($ch);
        }

        // Decode the JSON
        $result = json_decode($res, true);
        // is it valid JSON?
        if (!$result) {
            $return_status = -1001;
            return array('code' => -1001,
                'description' => 'Invalid data received, please contact Vennd.io support');
        } else {
            $return_status = $http_code;
        }

        return $result;
    }

    public function createPayment($destination_address, $amount, $asset, $payment_id, $tx_fee, &$return_status) {
        // generate the POST data json
        $post_data = array('destinationAddress' => $destination_address,
            'amount' => $amount,
            'asset' => $asset,
            'paymentId' => $payment_id,
            'txFee' => $tx_fee);

        $result = $this->postEnuAPI($this->baseURL()."/payment", $post_data, $return_status);

        // Expect HTTP status created (201)
        if ($return_status == 201) {
            $return_status = 0;
        }

        return $result;
    }

    public function getPayment($payment_id, &$return_status) {
        $result = $this->getEnuAPI($this->baseURL()."/payment/".$payment_id, $return_status);

        // Expect HTTP status OK (200)
        if ($return_status == 200) {
            $return_status = 0;
        }

        return $result;
    }

    public function retryPayment($payment_id, &$return_status) {
        // generate the POST data json
        $post_data = array(
            'paymentId' => $payment_id);

        $result = $this->postEnuAPI($this->baseURL()."/payment/status/".$payment_id, $post_data, $return_status);

        // Expect HTTP status OK (200)
        if ($return_status == 200) {
            $return_status = 0;
        }

        return $result;
    }

    public function createAddress(&$return_status) {
        // generate the POST data json
        $post_data = array();

        $result = $this->postEnuAPI($this->baseURL()."/address", $post_data, $return_status);

        // Expect HTTP status created (201)
        if ($return_status == 201) {
            $return_status = 0;
        }

        return $result;
    }
}

/**
 * Exceptions
 */
class EnuAPIException extends Exception {}
?>