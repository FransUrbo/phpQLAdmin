<?php
// control attribute plugin for
// relaymailfrom (allowed sender addresses for relaying)
//
// $Id: attrib.control.relaymailfrom.inc,v 2.9 2003-05-28 07:23:37 turbo Exp $

function relaymailfrom_check($type) {
	global $error, $relaymailfrom, $view;
	
	// check validity
	if(is_array($relaymailfrom)){
		foreach($relaymailfrom as $key => $badmail){
		 	if($badmail == ""){
				continue;
			}
			
			if(preg_match("/^@/", $badmail)){
				// retry with added user
				if(!check_email("user" . $badmail)){
					$is_error = true;
					$error["relaymailfrom_$key"] = PQL_LANG_INVALID;
				}
			} else {
				if(!check_email($badmail)){
					$is_error = true;
					$error["relaymailfrom_$key"] = PQL_LANG_INVALID;
				}
			} // end if preg_match...
		} // end foreach
	} // end if is_array
	
	if($is_error){
		return false;
	}
	
	return true;
}

function relaymailfrom_init($host) {
	// init values
	global $_pql_control, $relaymailfrom, $view;
	
	// fetch data from ldap server
	$relaymailfrom = pql_control_get_attribute($_pql_control->ldap_linkid,
											   "cn=" . $host . "," . $GLOBALS["USER_SEARCH_DN_CTR"],
											   "relaymailfrom");
	
	if(is_null($relaymailfrom)){
		return true;
	}
}

function relaymailfrom_print_view($host) {
	global $_pql_control, $relaymailfrom, $view;

	// init data
	relaymailfrom_init($host);
	
	if(is_array($relaymailfrom)){
		foreach($relaymailfrom as $key => $mail){
			if(preg_match("/^@/", $mail)){
				$mail = split("@", $mail);
				$relaymailfrom[$key] = "all of " . $mail[1];
			}
		}
	}
	
?>
  <table cellspacing="0" cellpadding="3" border="0">
    <th align="left">relaymailfrom (allowed senders for relaying)</th>
      <tr class="title">
        <td>Email</td>
      </tr>
<?php
	if(!is_array($relaymailfrom)) {
?>
      <tr class="<?php table_bgcolor(); ?>">
        <td>no address defined</td>
      </tr>
<?php
	} else {
		foreach($relaymailfrom as $mail){
?>
      <tr class="<?php table_bgcolor(); ?>">
        <td><?php echo $mail; ?></td>
      </tr>
<?php
		} // end foreach
	} // end if is_array
?>
      <tr class="subtitle">
        <td><a href="control_edit_attribute.php?mxhost=<?=$host?>&attrib=relaymailfrom"><img src="images/edit.png" width="12" height="12" border="0"> change options</a></td>
      </tr>
    </th>
  </table>
<?php
}

function relaymailfrom_print_form() {
	global $attrib, $PHP_SELF, $error, $relaymailfrom, $relaycount, $mxhost, $view;
	
	if(($relaycount < count($relaymailfrom) - 1) and count($relaymailfrom) != 0){
		$relaycount = count($relaymailfrom) - 1;
	}
	
?>
  <form action="<?php echo $PHP_SELF ?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left">relaymailfrom (allowed senders for relaying)</th>
        <tr class="title">
          <td>Address</td>
        </tr>

        <tr class="<?php table_bgcolor(); ?>">
          <td><?php echo format_error($error["relaymailfrom_0"]); ?><input type="text" name="relaymailfrom[0]" value="<?php echo $relaymailfrom[0]; ?>"></td>
        </tr>

<?php
  for($i = 1; $i <= $relaycount; $i++){
?>
        <tr class="<?php table_bgcolor(); ?>">
          <td><?php echo format_error($error["relaymailfrom_$i"]); ?><input type="text" name="relaymailfrom[<?=$i?>]" value="<?=$relaymailfrom[$i]?>"></td>
        </tr>

<?php	} ?>
        <tr class="subtitle">
          <td><a href="<?$PHP_SELF?>?attrib=<?$attrib?>&relaycount=<?=($relaycount+1)?>&mxhost=<?=$mxhost?>">add additional address</a> (please save first, changes will be lost)</td>
        </tr>

        <tr class="subtitle">
          <td>
            <img src="images/info.png" width="16" height="16" border="0">Note: this is a really bad idea, don't use this for a public server !!!
            <br> To allow relaying for a whole domain simply leave user empty (eg. @domain.com). To delete a relaying remove the value and save.
          </td>
        </tr>
      </th>
    </table>

    <input type="hidden" name="submit" value="1">
    <input type="hidden" name="attrib" value="<?php echo $attrib; ?>">
    <input type="hidden" name="mxhost" value="<?=$mxhost?>">
    <input type="hidden" name="view" value="<?=$view?>">
    <br>
    <input type="submit" value="<?php echo PQL_LANG_SAVE; ?>">
  </form>
<?php
}

function relaymailfrom_save($type, $host) {
	global $_pql_control, $attrib, $relaymailfrom, $error, $view;

	// recontrol values
	if(!relaymailfrom_check($type)){
 		return false;
	}

	switch($type){
		case "modify";
		case "add";
			// save entries
			if(pql_control_replace_attribute($_pql_control->ldap_linkid,
											 "cn=" . $host . "," . $GLOBALS["USER_SEARCH_DN_CTR"],
											 "relaymailfrom", $relaymailfrom)){
		      	$msg = "Successfully saved relaymailfrom options";
			} else {
				$msg = "Failed saving relaymailfrom: " . pql_ldap_error($_pql_control->ldap_linkid);
	    	}

			attribute_forward($msg);
		    break;
		default:
			die("unknown save type $type in " . __FILE__ . ", function save()");
	}
}

function relaymailfrom_help(){
?>
This file contains envelope sender addresses that are allowed to relay through this server. This is a really bad idea as sender addresses are very easy to spoof and you are an
open relay then. You should use SMTP after POP instead.
<br><br>
    joe@yourdomain.com<br>
    @otherdomain.com
<br><br>
The first example allows joe@yourdomain.com to relay, the second one allows all addresses ending with @otherdomain.com to relay.
<?php
}

function relaymailfrom_help_cr(){
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
?>
