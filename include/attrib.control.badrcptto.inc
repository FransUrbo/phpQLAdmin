<?php
// control attribute plugin for
// badrcptto (blocked local addresses)
//
// $Id: attrib.control.badrcptto.inc,v 2.10 2003-05-28 07:23:37 turbo Exp $

function badrcptto_check($type) {
	global $error, $badrcptto, $view;

	// check validity
	if(is_array($badrcptto)){
		foreach($badrcptto as $key => $badmail){
		 	if($badmail == ""){
				continue;
			}
			
			if(!check_email($badmail)){
				$is_error = true;
				$error["badrcptto_$key"] = PQL_LANG_INVALID;
			}
		} // end foreach
	} // end if is_array

	if($is_error){
		return false;
	}
	
	return true;
}

// init values
function badrcptto_init($host) {
	global $_pql_control, $badrcptto, $view;

	// fetch data from ldap server
	$badrcptto = pql_control_get_attribute($_pql_control->ldap_linkid,
										   "cn=" . $host . "," . $GLOBALS["USER_SEARCH_DN_CTR"],
										   "badrcptto");

	if(is_null($badrcptto)){
		return true;
	}
}

function badrcptto_print_view($host) {
	global $_pql_control, $badrcptto, $view;

	// init data
	badrcptto_init($host);

	if(is_array($badrcptto)){
		foreach($badrcptto as $key => $mail){
			if(preg_match("/^@/", $mail)){
				$mail = split("@", $mail);
				$badrcptto[$key] = "all of " . $mail[1];
			}
		}
	}
	
?>
  <table cellspacing="0" cellpadding="3" border="0">
    <th align="left">badrcptto (rejected local addresses)</th>
      <tr class="title">
        <td>Email</td>
      </tr>

<?php
	if(!is_array($badrcptto)) {
?>
      <tr class="<?php table_bgcolor(); ?>">
        <td>no address defined</td>
      </tr>
<?php
	} else {
		foreach($badrcptto as $mail){
?>
      <tr class="<?php table_bgcolor(); ?>">
        <td><?php echo $mail; ?></td>
      </tr>

<?php
		} // end foreach
	} // end if is_array
?>
      <tr class="subtitle">
        <td><a href="control_edit_attribute.php?mxhost=<?=$host?>&attrib=badrcptto"><img src="images/edit.png" width="12" height="12" border="0"> change options</a></td>
      </tr>
    </th>
  </table>

<?php
}

function badrcptto_print_form(){
	global $attrib, $PHP_SELF, $error, $badrcptto, $badcount, $mxhost, $view;

	if(($badcount < count($badrcptto) - 1) and count($badrcptto) != 0){
		$badcount = count($badrcptto) - 1;
	}

?>
  <form action="<?php echo $PHP_SELF ?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left">badrcptto (rejected local addresses)</th>
        <tr class="title">
          <td>Address</td>
        </tr>

        <tr class="<?php table_bgcolor(); ?>">
          <td><?php echo format_error($error["badrcptto_0"]); ?><input type="text" name="badrcptto[0]" value="<?php echo $badrcptto[0]; ?>"></td>
        </tr>

<?php for($i = 1; $i <= $badcount; $i++){ ?>
        <tr class="<?php table_bgcolor(); ?>">
          <td><?php echo format_error($error["badrcptto_$i"]); ?><input type="text" name="badrcptto[<?php echo $i; ?>]" value="<?php echo $badrcptto[$i]; ?>"></td>
        </tr>

<?php } ?>
        <tr class="subtitle">
          <td><a href="<?=$PHP_SELF?>?attrib=<?=$attrib?>&badcount=<?=($badcount+1)?>&mxhost=<?=$mxhost?>">add additional address</a> (please save first, changes will be lost)</td>
        </tr>

        <tr class="subtitle">
          <td><img src="images/info.png" width="16" height="16" border="0">To delete an address remove the value and save.</td>
        </tr>
      </th>
    </table>

    <input type="hidden" name="submit" value="1">
    <input type="hidden" name="attrib" value="<?php echo $attrib; ?>">
    <input type="hidden" name="mxhost" value="<?=$mxhost?>">
    <input type="submit" value="<?php echo PQL_LANG_SAVE; ?>">
  </form>
<?php
}

function badrcptto_save($type, $host) {
	global $_pql_control, $attrib, $badrcptto, $error, $mxhost, $view;

	// recontrol values
	if(!badrcptto_check($type)){
 		return false;
	}

	switch($type){
	  case "modify";
	  case "add";
		// save entries
		
		if(pql_control_replace_attribute($_pql_control->ldap_linkid,
										 "cn=" . $mxhost . "," . $GLOBALS["USER_SEARCH_DN_CTR"], "badrcptto",
										 $badrcptto)) {
			$msg = "Successfully saved badrcptto options";
		} else {
			$msg = "Failed saving badrcptto: " . pql_ldap_error($_pql_control->ldap_linkid);
    	}

		attribute_forward($msg);
		break;
	  default:
		die("unknown save type $type in " . __FILE__ . ", function save()");
	}
}

function badrcptto_help(){
?>
Contains a list of local recipient addresses that are rejected. If the sender has RELAYCLIENT="" set this file has no effect.
<?php
}

function badrcptto_help_cr(){
?>
	help text taken from life with qmail-ldap
<?php
}
/*
 * Local variables:
 * mode: php
 * mode: font-lock
 * tab-width: 4
 * End:
 */
