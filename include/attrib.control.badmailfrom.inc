<?php
// control attribute plugin for
// badmailfrom (blocked from addresses)
//
// $Id: attrib.control.badmailfrom.inc,v 2.0 2002-12-13 14:34:57 turbo Exp $

function badmailfrom_check($type){
	global $error, $badmailfrom;

	// check validity
	if(is_array($badmailfrom)){
		foreach($badmailfrom as $key => $badmail){
		 	if($badmail == ""){
				continue;
    	}

      if(preg_match("/^@/", $badmail)){
				// retry with added user
				if(!check_email("user" . $badmail)){
					$is_error = true;
					$error["badmailfrom_$key"] = PQL_INVALID;
				}
			} else {
				if(!check_email($badmail)){
					$is_error = true;
					$error["badmailfrom_$key"] = PQL_INVALID;
				}
			} // end if preg_match...

		} // end foreach

	} // end if is_array

	if($is_error){
		return false;
  }

	return true;
}

function badmailfrom_init(){
	// init values
  global $_pql_control, $badmailfrom;

	// fetch data from ldap server
	$badmailfrom = pql_control_get_attribute($_pql_control->ldap_linkid, PQL_LDAP_CONTROL_BASEDN, "badmailfrom");

	if(is_null($badmailfrom)){
		return true;
	}

}

function badmailfrom_print_view(){
	global $_pql_control, $badmailfrom;

	// init data
	badmailfrom_init();

	if(is_array($badmailfrom)){
		foreach($badmailfrom as $key => $mail){
			if(preg_match("/^@/", $mail)){
				$mail = split("@", $mail);
				$badmailfrom[$key] = "all of " . $mail[1];
    	}
		}
	}

	?>
	<table cellspacing="0" cellpadding="3" border="0">
	<th align="left">badmailfrom (Blocked senders)</th>
	<tr class="title">
		<td>Email</td>
  </tr>
  <?php
	if(!is_array($badmailfrom)){
		?>
	<tr class="<?php table_bgcolor(); ?>">
		<td>no address defined</td>
	</tr>
		<?php
	} else {

		foreach($badmailfrom as $mail){
	?>
	<tr class="<?php table_bgcolor(); ?>">
		<td><?php echo $mail; ?></td>
	</tr>
	<?php
		} // end foreach
	} // end if is_array
	?>
	<tr class="subtitle">
		<td><a href="control_edit_attribute.php?attrib=badmailfrom"><img src="images/edit.png" width="12" height="12" border="0"> change options</a></td>
	</tr>
	</table>

	<?php
}

function badmailfrom_print_form(){
	global $attrib, $PHP_SELF, $error, $badmailfrom, $badcount;

	if(($badcount < count($badmailfrom) - 1) and count($badmailfrom) != 0){
		$badcount = count($badmailfrom) - 1;
	}

	?>
	<form action="<?php echo $PHP_SELF ?>" method="post">
	<table cellspacing="0" cellpadding="3" border="0">
	<th colspan="3" align="left">badmailfrom (Blocked senders)</th>
	<tr class="title">
		<td>Address</td>
	</tr>
	<tr class="<?php table_bgcolor(); ?>">
		<td><?php echo format_error($error["badmailfrom_0"]); ?><input type="text" name="badmailfrom[0]" value="<?php echo $badmailfrom[0]; ?>"></td>
	</tr>
	<?php for($i = 1; $i <= $badcount; $i++){ ?>
	<tr class="<?php table_bgcolor(); ?>">
		<td><?php echo format_error($error["badmailfrom_$i"]); ?><input type="text" name="badmailfrom[<?php echo $i; ?>]" value="<?php echo $badmailfrom[$i]; ?>"></td>
	</tr>
	<?php } ?>
	<tr class="subtitle">
		<td><a href="<?php echo $PHP_SELF; ?>?attrib=<?php echo $attrib; ?>&badcount=<?php echo ($badcount + 1); ?>">add additional address</a> (please save first, changes will be lost)</td>
	</tr>
	<tr class="subtitle">
		<td><img src="images/info.png" width="16" height="16" border="0"> To block a whole domain simply leave user empty (eg. @domain.com). To delete a blocked address remove the value and save.</td>
	</tr>
	</table>
	<input type="hidden" name="submit" value="1">
	<input type="hidden" name="attrib" value="<?php echo $attrib; ?>">
	<br>
	<br>
	<input type="submit" value="<?php echo PQL_SAVE; ?>">
	</form>

	<?php
}

function badmailfrom_save($type){
	global $_pql_control, $attrib, $badmailfrom, $error;

	// recontrol values
	if(!badmailfrom_check($type)){
 		return false;
	}

	switch($type){

		case "modify";
			// save entries

			if(pql_control_replace_attribute($_pql_control->ldap_linkid, PQL_LDAP_CONTROL_BASEDN, "badmailfrom", $badmailfrom)){
      	$msg = "Successfully saved badmailfrom options";
			} else {
				$msg = "Failed saving badmailfrom: " . pql_ldap_error($_pql_control->ldap_linkid);
    	}

			attribute_forward($msg);
      break;
   	default:
			die("unknown save type $type in " . __FILE__ . ", function save()");
	}

}

function badmailfrom_help(){
?>
	Unacceptable envelope sender addresses.  qmail-smtpd
	will reject every recipient address for a message if
	the envelope sender address is listed in badmailfrom.
	A line in badmailfrom may be of the form @host, meaning
	every address at host.
<?php
}

function badmailfrom_help_cr(){
?>
	help text taken from qmail man pages
<?php
}