<?php
// control attribute plugin for
// rcpthosts (blocked local addresses)
//
// $Id: attrib.control.rcpthosts.inc,v 2.6 2003-05-01 10:01:04 turbo Exp $

function rcpthosts_check($type){
    global $error, $rcpthosts, $host;
    
    // check validity
    if(is_array($rcpthosts)){
		foreach($rcpthosts as $key => $host){
			if($host == ""){
				continue;
			}
			
			if(!check_hostaddress($host)){
				$error["rcpthosts_$key"] = PQL_LANG_INVALID;
				$is_error = true;
			}
		} // end foreach
    } // end if is_array
	
    if($is_error){
		return false;
    }
    
    return true;
}

// init values
function rcpthosts_init($host){
    global $_pql_control, $rcpthosts;
    
    // fetch data from ldap server
    $rcpthosts = pql_control_get_attribute($_pql_control->ldap_linkid,
										   "cn=" . $host . "," . $GLOBALS["USER_SEARCH_DN_CTR"],
										   "rcpthosts");
    
    if(is_null($rcpthosts))
	  return true;
}

function rcpthosts_print_view($host) {
    global $_pql_control, $rcpthosts;
    
    // init data
    rcpthosts_init($host);
    
    if(is_array($rcpthosts)){
		foreach($rcpthosts as $key => $mail){
			if(preg_match("/^@/", $mail)){
				$mail = split("@", $mail);
				$rcpthosts[$key] = "all of " . $mail[1];
			}
		}
    }
?>

  <table cellspacing="0" cellpadding="3" border="0">
    <th align="left">rcpthosts (allowed rcpt hosts) for host <u><?=$host?></u></th>
      <tr class="title">
        <td>Host</td>
      </tr>

<?php
      if(!is_array($rcpthosts)){
?>
      <tr class="<?php table_bgcolor(); ?>">
        <td>no address defined</td>
      </tr>
<?php
      } else {
		  asort($rcpthosts);
		  foreach($rcpthosts as $rcpt){
?>
      <tr class="<?php table_bgcolor(); ?>">
        <td><?php echo $rcpt; ?></td>
      </tr>

<?php
          } // end foreach
      } // end if is_array
?>
      <tr class="subtitle">
        <td>
          <a href="control_edit_attribute.php?attrib=rcpthosts&mxhost=<?=$host?>">
          <img src="images/edit.png" width="12" height="12" border="0">change options</a>
        </td>
      </tr>
    </th>
  </table>
<?php
}

function rcpthosts_print_form(){
    global $attrib, $PHP_SELF, $error, $rcpthosts, $mxhost;
?>
  <form action="<?php echo $PHP_SELF ?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left">Add domain:</th>
        <tr class="<?php table_bgcolor(); ?>">
          <td><input type="text" name="set""></td>
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

function rcpthosts_save($type, $host) {
    global $_pql_control, $attrib, $rcpthosts, $set, $error;
    
    // recontrol values
    if(!rcpthosts_check($type))
	  return false;

    switch($type) {
      case "del":
		// delete entry
		
		// remove specified domain from rcpthosts, redirect back to host details
		if(pql_control_remove_attribute($_pql_control->ldap_linkid,
										"cn=" . $host . "," . $GLOBALS["USER_SEARCH_DN_CTR"],
										"rcpthosts", $set))
		  $msg = "Successfully removed $set from rcpthosts";
		else
		  $msg = "Failed removing $set from rcpthosts: " . pql_ldap_error($_pql_control->ldap_linkid);
		
		attribute_forward($msg);
		break;

      case "add":
		// add entry
		
		// adding specified domain to rcpthosts, redirect back to domain details
		if(pql_control_add_attribute($_pql_control->ldap_linkid,
									 "cn=" . $host . "," . $GLOBALS["USER_SEARCH_DN_CTR"],
									 "rcpthosts", $set))
			$msg = "Successfully added $set to rcpthosts";
		else
			$msg = "Failed adding $set to rcpthosts: " . pql_ldap_error($_pql_control->ldap_linkid);
		
		attribute_forward($msg);
		break;

      default:
		die("unknown save type $type in " . __FILE__ . ", function save()");
    }
}

function rcpthosts_help(){
?>
Allowed RCPT domains.  If rcpthosts is supplied,
qmail-smtpd will reject any envelope recipient address
with a domain not listed in rcpthosts.
<br>
Exception:  If the environment variable RELAYCLIENT is
set, qmail-smtpd will ignore rcpthosts, and will append
the value of RELAYCLIENT to each incoming recipient
address.
<br><br>
rcpthosts may include wildcards:
<br><br>
		heaven.af.mil<br>
		.heaven.af.mil<br>
<br>
Envelope recipient addresses without @ signs are always
allowed through.
<?php
}

function rcpthosts_help_cr(){
?>
	help text taken from qmail man pages
<?php
/*
 * Local variables:
 * mode: php
 * mode: font-lock
 * tab-width: 4
 * End:
 */
}
