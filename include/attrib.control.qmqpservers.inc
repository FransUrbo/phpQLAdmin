<?php
// control attribute plugin for
// qmqpservers (max number of bytes in message)
//
// $Id: attrib.control.qmqpservers.inc,v 2.3 2002-12-18 16:28:46 turbo Exp $

function qmqpservers_check($type) {
	global $error, $qmqpservers;

	// check each field
	if(is_array($qmqpservers)){
		foreach($qmqpservers as $key => $server){
			if($server == ""){
				continue;
			}
			
			// it is an ip address
			if(!preg_match("/([0-9]{1,3}\.){3,3}[0-9]{1,1}/", $server)){
				$is_error = true;
				$error["qmqpservers_$key"] = PQL_INVALID;
			} else {
				// ip format ok, check validity of matched ip address
				$ip = explode(".", $server);
				
				foreach($ip as $ip_key => $ip_part){
					switch((int)$ip_key){
					  case 0:
					  case 3:
						if($ip_part <= 0 or $ip_part > 254){
							$is_error = true;
							$error["qmqpservers_$key"] = PQL_INVALID;
						}
						break;
						
					  default:
						if($ip_part < 0 or $ip_part > 255){
							$is_error = true;
							$error["qmqpservers_$key"] = PQL_INVALID;
						}
					} // end switch
				} // end foreach
			} // end if !preg_match...
		} // end foreach
	} // end if is_array...

	if($is_error){
		return false;
	}
	
	return true;
}

function qmqpservers_init($host) {
	// init values
	global $_pql_control, $qmqpservers;

	// fetch data from ldap server
	$qmqpservers = pql_control_get_attribute($_pql_control->ldap_linkid,
											 "cn=" . $host . "," . $GLOBALS["USER_SEARCH_DN_CTR"],
											 "qmqpservers");

	if(is_null($qmqpservers)){
		return true;
	}
}

function qmqpservers_print_view($host) {
	global $_pql_control, $qmqpservers;

	// init data
	qmqpservers_init($host);

?>
  <table cellspacing="0" cellpadding="3" border="0">
    <th align="left">qmqpservers (QMQP servers)</th>
      <tr class="title">
        <td>IP</td>
      </tr>
<?php
	if(!is_array($qmqpservers)){
?>
      <tr class="<?php table_bgcolor(); ?>">
        <td>no ip defined</td>
      </tr>
<?php
	} else {
		foreach($qmqpservers as $server){
?>
      <tr class="<?php table_bgcolor(); ?>">
        <td><?php echo $server; ?></td>
      </tr>
<?php
		} // end foreach
	} // end if is_array
?>
      <tr class="subtitle">
        <td><a href="control_edit_attribute.php?attrib=qmqpservers"><img src="images/edit.png" width="12" height="12" border="0"> change options</a></td>
      </tr>
    </th>
  </table>

<?php
}

function qmqpservers_print_form() {
	global $attrib, $PHP_SELF, $error, $qmqpservers, $qmqpcount;

	if(($qmqpcount < count($qmqpservers) - 1) and count($qmqpservers) != 0){
		$qmqpcount = count($qmqpservers) - 1;
	}

?>
  <form action="<?php echo $PHP_SELF ?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left">qmqpservers (QMQP servers)</th>
        <tr class="title">
          <td>IP Address</td>
        </tr>

        <tr class="<?php table_bgcolor(); ?>">
          <td><?php echo format_error($error["qmqpservers_0"]); ?><input type="text" name="qmqpservers[0]" value="<?php echo $qmqpservers[0]; ?>"></td>
        </tr>

<?php for($i = 1; $i <= $qmqpcount; $i++){ ?>
        <tr class="<?php table_bgcolor(); ?>">
          <td><?php echo format_error($error["qmqpservers_$i"]); ?><input type="text" name="qmqpservers[<?php echo $i; ?>]" value="<?php echo $qmqpservers[$i]; ?>"></td>
        </tr>

<?php } ?>
        <tr class="subtitle">
          <td><a href="<?php echo $PHP_SELF; ?>?attrib=<?php echo $attrib; ?>&qmqpcount=<?php echo ($qmqpcount + 1); ?>">add additional address</a> (please save first, changes will be lost)</td>
        </tr>

        <tr class="subtitle">
          <td><img src="images/info.png" width="16" height="16" border="0"> To delete a QMQP server remove the value and save.</td>
        </tr>
      </th>
    </table>

    <input type="hidden" name="submit" value="1">
    <input type="hidden" name="attrib" value="<?php echo $attrib; ?>">
    <input type="submit" value="<?php echo PQL_SAVE; ?>">
  </form>

<?php
}

function qmqpservers_save($type, $save) {
	global $_pql_control, $attrib, $qmqpservers, $error;

	// recontrol values
	if(!qmqpservers_check($type)){
 		return false;
	}

	switch($type) {
	  case "modify";
		// save entries
		if(pql_control_replace_attribute($_pql_control->ldap_linkid,
										 "cn=" . $host . "," . $GLOBALS["USER_SEARCH_DN_CTR"],
										 "qmqpservers", $qmqpservers)){
			$msg = "Successfully saved qmqpservers options";
		} else {
			$msg = "Failed saving qmqpservers: " . pql_ldap_error($_pql_control->ldap_linkid);
    	}

		attribute_forward($msg);
		break;
	  default:
		die("unknown save type $type in " . __FILE__ . ", function save()");
	}
}

function qmqpservers_help(){
?>
	IP addresses of QMQP servers, one address per line.
	qmail-qmqpc will try each address in turn until it
	establishes a QMQP connection or runs out of addresses.
<?php
}

function qmqpservers_help_cr(){
?>
	help text taken from qmail man pages
<?php
}
/*
 * Local variables:
 * mode: php
 * mode: font-lock
 * tab-width: 4
 * End:
 */
