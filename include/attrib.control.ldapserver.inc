<?php
// control attribute plugin for
// ldapserver (server which holds ldap database)
// ldapbasedn (dn, which points to ldap database)
// ldaplogin (dn of a database user)
// ldappassword (password for bind - used with ldaplogin)
// ldapgrouplogin (dn of a database user - used by qmail-group)
// ldapgrouppassword (password for bind - used with ldapgrouplogin)
//
// $Id: attrib.control.ldapserver.inc,v 2.17 2004-10-19 10:40:40 turbo Exp $

// {{{ ldapserver_check(type)
function ldapserver_check($type) {
	global $error, $LANG;

	// missing
	if($_REQUEST["ldapserver"][0] == "") {
		$error["ldapserver_0"] = $LANG->_('Missing');
		$is_error = true;
	}

	// check for valid fqdn for all servers
	if(is_array($_REQUEST["ldapserver"])) {
		foreach($_REQUEST["ldapserver"] as $key => $server) {
			if($server != "" and !pql_check_hostaddress($server, true)) {
				$error["ldapserver_$key"] = $LANG->_('Invalid');
				$is_error = true;
			}
		}
	}
	
	// missing
	if($_REQUEST["ldapbasedn"] == "") {
		$error["ldapbasedn"] = $LANG->_('Missing');
		$is_error = true;
	}
	
	// check for valid dn
	
	// login / password
	if($_REQUEST["ldaplogin_use"] == "1") {
		// missing
		if($_REQUEST["ldaplogin"] == "") {
			$error["ldaplogin"] = $LANG->_('Missing');
			$is_error = true;
		}
		
		// check for valid dn
		
		// missing
		if($_REQUEST["ldappassword"] == "") {
			$error["ldappassword"] = $LANG->_('Missing');
			$is_error = true;
		}
	}

	// test connection
	if($_REQUEST["ldap_test"] == "1" and $is_error == false) {
		foreach($_REQUEST["ldapserver"] as $server) {
			if($server == "")
			  continue;
			
			$linkid = @ldap_connect($server);
			
			if($linkid) {
				if($_REQUEST["ldaplogin_use"] == "1") {
					if(!@ldap_bind($linkid, $_REQUEST["ldaplogin"], $_REQUEST["ldappassword"])) {
						$error["ldap_test"] .= pql_complete_constant($LANG->_('Failed running a test connection to %server%'),
																	 array('server' => $server)) . ": " . ldap_error($linkid) . "<br>";
						$is_error = true;
					}
				} else {
					if(!@ldap_bind($linkid)) {
						$error["ldap_test"] .= pql_complete_constant($LANG->_('Failed running a test connection to %server%'),
																	 array('server' => $server)) . ": " . ldap_error($linkid) . "<br>";
						$is_error = true;
					}
				}
			} else {
				// connection failed
				$error["ldap_test"] .= pql_complete_constant($LANG->_('Failed running a test connection to %server%'),
															 array('server' => $server))
				  . '(' . $LANG->_('server not responding or invalid name') . ')<br>';
				$is_error = true;
			}
			
			// search for base dn
			if($is_error == false)
			  // TODO: How to check basedn???
			
			@ldap_unbind($linkid);
			@ldap_close($linkid);
		}
	}

	if($is_error == false)
	  return true;
}
// }}}

// {{{ ldapserver_init(host)
// init values
function ldapserver_init($host) {
	global $_pql_control;
	
	// fetch data from ldap server
	$attribs = array("ldapserver"   	=> pql_get_define("PQL_ATTR_LDAPSERVER"),
					 "ldapbasedn"   	=> pql_get_define("PQL_ATTR_LDAPBASEDN"),
					 "ldaplogin"    	=> pql_get_define("PQL_ATTR_LDAPLOGIN"),
					 "ldappassword"		=> pql_get_define("PQL_ATTR_LDAPPASSWORD"),
					 "ldapgrouplogin"	=> pql_get_define("PQL_ATTR_GROUP_LOGIN"),
					 "ldapgrouppassword"=> pql_get_define("PQL_ATTR_GROUP_PASSWD"));
	foreach($attribs as $key => $attrib) {
		$_REQUEST[$key] = pql_get_attribute($_pql_control->ldap_linkid,
											pql_get_define("PQL_ATTR_CN")."=$host,".$_SESSION["USER_SEARCH_DN_CTR"],
											$attrib);
	}
	
	// init checkbox
	if($_REQUEST["ldaplogin"])
	  $_REQUEST["ldaplogin_use"] = 1;
	
	// map different hostnames sperated by space to an array
	$_REQUEST["ldapserver"] = split(" ", $_REQUEST["ldapserver"]);
	if($_REQUEST["srvcount"] == "")
	  $_REQUEST["srvcount"] = count($_REQUEST["ldapserver"]) - 1;
}
// }}}


// {{{ ldapserver_print_view(host)
function ldapserver_print_view($host) {
	global $_pql_control, $LANG;

	// init data
	ldapserver_init($host);

	// don't display passwords
	if($_REQUEST["ldappassword"] != "")
	  $_REQUEST["ldappassword"] = $LANG->_('Set, but now shown');

	// set empty field to "not set"
	$attribs = array("ldapserver"   	=> pql_get_define("PQL_ATTR_LDAPSERVER"),
					 "ldapbasedn"   	=> pql_get_define("PQL_ATTR_LDAPBASEDN"),
					 "ldaplogin"    	=> pql_get_define("PQL_ATTR_LDAPLOGIN"),
					 "ldappassword" 	=> pql_get_define("PQL_ATTR_LDAPPASSWORD"),
					 "ldapgrouplogin"	=> pql_get_define("PQL_ATTR_GROUP_LOGIN"),
					 "ldapgrouppassword"=> pql_get_define("PQL_ATTR_GROUP_PASSWD"));
	foreach($attribs as $key => $attrib) {
		if($_REQUEST[$key] == "")
		  $_REQUEST[$key] = $LANG->_('Not set');
	}
?>
  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left"><?=$LANG->_('Server connection options')?></th>
      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('Host(s)')?></td>
        <td><?php echo join(", ", $_REQUEST["ldapserver"]); ?></td>
      </tr>

      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('Base DN')?></td>
        <td><?=$_REQUEST["ldapbasedn"]?></td>
      </tr>

      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('Login')?></td>
        <td><?=$_REQUEST["ldaplogin"]?></td>
      </tr>

      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('Password')?></td>
        <td><?=$_REQUEST["ldappassword"]?></td>
      </tr>

<?php if($_REQUEST["ldapserver"] == $LANG->_('Not set') or $_REQUEST["ldapbasedn"] == $LANG->_('Not set')) { ?>
      <tr class="subtitle">
        <td colspan="2"><span class="error"><?=$LANG->_('You have to set both, host and base dn to make qmail-ldap work')?>.</td>
      </tr>

<?php } ?>
      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('Group login')?></td>
        <td><?=$_REQUEST["ldapgrouplogin"]?></td>
      </tr>

      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('Group login password')?></td>
        <td><?=$_REQUEST["ldapgrouppassword"]?></td>
      </tr>

      <tr class="subtitle">
        <td colspan="2"><a href="control_edit_attribute.php?mxhost=<?=$host?>&attrib=ldapserver"><img src="images/edit.png" width="12" height="12" border="0"><?=$LANG->_('Change options')?></a></td>
      </tr>
    </th>
  </table>
<?php
}
// }}}

// {{{ ldapserver_print_form(void)
function ldapserver_print_form() {
	global $error, $LANG;
?>
  <form action="<?=$_SERVER["PHP_SELF"]?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left"><?=$LANG->_('LDAP server connection option')?></th>
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Host(s)')?></td>
          <td><?php echo pql_format_error_span($error["ldapserver_0"]); ?><input type="text" name="ldapserver[0]" value="<?=$_REQUEST["ldapserver"][0]?>">
		    <br>
<?php for($i = 1; $i <= $_REQUEST["srvcount"]; $i++){ ?>
			<?php echo pql_format_error_span($error["ldapserver_$i"]); ?><input type="text" name="ldapserver[<?=$i?>]" value="<?=$_REQUEST["ldapserver"][$i]?>"><br>
<?php } ?>
            <br>
            <a href="<?=$_SERVER["PHP_SELF"]?>?attrib=<?=$_REQUEST["attrib"]?>&srvcount=<?php echo ($_REQUEST["srvcount"] + 1); ?>"><?php echo pql_complete_constant($LANG->_('Add %what%'), array('what' => $LANG->_('more servers'))); ?></a>
          </td>
        </tr>

        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Base DN')?></td>
          <td><?php echo pql_format_error_span($error["ldapbasedn"]); ?><input type="text" name="ldapbasedn" value="<?=$_REQUEST["ldapbasedn"]?>"></td>
        </tr>

        <tr class="<?php pql_format_table(); ?>">
          <td colspan="2"><input type="checkbox" name="ldaplogin_use" value="1" <?php if($_REQUEST["ldaplogin_use"] == "1"){ echo "checked"; } ?>> <?=$LANG->_('Use login and password')?></td>
        </tr>

        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Login (valid dn)')?></td>
          <td><?php echo pql_format_error_span($error["ldaplogin"]); ?><input type="text" name="ldaplogin" value="<?=$_REQUEST["ldaplogin"]?>"></td>
        </tr>

        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Password')?></td>
          <td><?php echo pql_format_error_span($error["ldappassword"]); ?><input type="password" name="ldappassword" value="<?=$_REQUEST["ldappassword"]?>"></td>
        </tr>

        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Group login')?></td>
          <td><?php echo pql_format_error_span($error["ldapgrouplogin"]); ?><input type="text" name="ldapgrouplogin"></td>
        </tr>

        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Group login password')?></td>
          <td><?php echo pql_format_error_span($error["ldapgrouppassword"]); ?><input type="password" name="ldapgrouppassword"></td>
        </tr>

        <tr class="<?php pql_format_table(); ?>">
          <td colspan="2"><input type="checkbox" name="ldap_test" value="1" <?php if($_REQUEST["ldap_test"] == "1"){ echo "checked"; } //" ?>>
			<?=$LANG->_('Test connection after submitting form (it will be tested for each server seperatly)')?>
          </td>
        </tr>

<?php if($error["ldap_test"] != ""){ ?>
        <tr class="<?php pql_format_table(); ?>">
          <td colspan="2"><?php echo pql_format_error_span($error["ldap_test"]); ?></td>
        </tr>
<?php } ?>
      </th>
    </table>

    <input type="hidden" name="submit"   value="1">
    <input type="hidden" name="srvcount" value="<?=$_REQUEST["srvcount"]?>">
    <input type="hidden" name="attrib"   value="<?=$_REQUEST["attrib"]?>">
    <input type="hidden" name="mxhost"   value="<?=$_REQUEST["mxhost"]?>">
    <input type="hidden" name="view"     value="<?=$_REQUEST["view"]?>">
    <br>
    <input type="submit" value="<?=$LANG->_('Save')?>">
  </form>
<?php
}
// }}}

// {{{ ldapserver_save(type, host)
function ldapserver_save($type, $host) {
	global $_pql_control, $error, $LANG;
	
	// map ldapserver array to a single string
	$ldapserver = trim(join(" ", $_REQUEST["ldapserver"]));
	
	// remove all double, triple or whatever spaces, replace with single one
	$ldapserver = preg_replace("/\s+/", " ", $ldapserver);
	
	switch($type) {
		case "modify";
		case "add";
		// save entries
		$attribs = array("ldapserver"   => pql_get_define("PQL_ATTR_LDAPSERVER"),
						 "ldapbasedn"   => pql_get_define("PQL_ATTR_LDAPBASEDN"));
		foreach($attribs as $key => $attrib)
		  $return[] = pql_modify_attribute($_pql_control->ldap_linkid,
										   pql_get_define("PQL_ATTR_CN")."=$host,".$_SESSION["USER_SEARCH_DN_CTR"],
										   $attrib, '', $_REQUEST[$key]);
		

		$attribs = array("ldaplogin"    	=> pql_get_define("PQL_ATTR_LDAPLOGIN"),
						 "ldappassword" 	=> pql_get_define("PQL_ATTR_LDAPPASSWORD"),
						 "ldapgrouplogin"	=> pql_get_define("PQL_ATTR_GROUP_LOGIN"),
						 "ldapgrouppassword"=> pql_get_define("PQL_ATTR_GROUP_PASSWD"));
		if($_REQUEST["ldaplogin_use"] == "1") {
			foreach($attribs as $key => $attrib)
			  if($_REQUEST[$key]) {
				  // Only set a value which is defined to avoid possible errors
				  $return[] = pql_modify_attribute($_pql_control->ldap_linkid,
												   pql_get_define("PQL_ATTR_CN")."=$host,".$_SESSION["USER_SEARCH_DN_CTR"],
												   $attrib, '', $_REQUEST[$key]);
			  }
		} else {
			foreach($attribs as $key => $attrib)
			  // reset login and password values
			  $return[] = pql_modify_attribute($_pql_control->ldap_linkid,
											   pql_get_define("PQL_ATTR_CN")."=$host,".$_SESSION["USER_SEARCH_DN_CTR"],
											   $attrib, '', '');
    	}

		if(in_array(false, $return))
		  $msg = pql_complete_constant($LANG->_('Failed to change %what%'),
									   array('what' => $LANG->_('ldapserver options')))
			. ": " . pql_format_error(0);
		else
		  $msg = pql_complete_constant($LANG->_('Successfully changed %what%'),
									   array('what' => $LANG->_('ldapserver options')));

		attribute_forward($msg);
		break;

	  default:
		die(pql_complete_constant($LANG->_('Unknown save type %type% in file %file%, function save()'),
								  array('type' => $type, 'file' => __FILE__)));
	}
}
// }}}


// {{{ ldapserver_help(void)
function ldapserver_help() {
	global $LANG;
?>
  <b><?=$LANG->_('Host')?></b><br>
	Your ldap server's hostname. If you want more than one ldap-server for redundancy,
	use the "add more servers" link to add another server. To clear additional server simply
	remove the value and save.
	<br><br>
  <b><?=$LANG->_('Base DN')?></b><br>
    The BaseDN for ldap searches. See OpenLDAP's documentation for more information about BaseDN. Required.
    <br>
    <i>Example: <u>o=yourcorp,c=ch</u></i>
	<br><br>
  <b><?=$LANG->_('Login')?></b><br>
	If you need to authentificate against your ldap server to retrieve the user information, this is the username to do so.
    <br>
	<i>Note1: This is a ldap dn, not a unix username.</i>
    <br>
	<i>Note2: If unset, all searches will be done anonymously so make sure everyone can read the relevant values.</i>
	<br><br>
  <b><?=$LANG->_('Password')?></b><br>
	The password for the user defined in ldaplogin if needed.
    <br>
    <i>Note: This should be entered as cleartext, not encrypted!</i>
	<br><br>
  <b><?=$LANG->_('Group login')?></b><br>
	If qmail-group need to authentificate against your ldap server to retrieve the user information, this is the username
    to do so.
    <br>
	<i>Note1: This is a ldap dn, not a unix username.</i>
    <br>
	<i>Note2: If unset, it will default to the value in ldaplogin.</i>
	<br><br>
  <b><?=$LANG->_('Group login password')?></b><br>
	The password for the user defined in ldapgrouplogin if needed.
    <br>
    <i>Note1: This should be entered as cleartext, not encrypted!</i>
    <br>
	<i>Note2: If unset, it will default to the value in ldappassword.</i>
<?php
}
// }}}

// {{{ ldapserver_help_cr(void)
function ldapserver_help_cr() {
	global $LANG;
	echo $LANG->_('Help text taken from life with qmail-ldap');
}
// }}}

// Local variables:
// mode: php
// mode: font-lock
// tab-width: 4
// End:
?>
