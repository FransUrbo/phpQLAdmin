<?php
// attribute plugin for
// mailhost (host address which resides the mailhost)
// $Id: attrib.mailhost.inc,v 2.25 2007-07-13 11:12:04 turbo Exp $

// {{{ attribute_check(type)
function attribute_check($type) {
    global $error, $LANG;
	
	// clear attribute ?
	if($_REQUEST["options"] == "none")
	  $_REQUEST["mailhost"] = "";
	
	// is typed in ?
	if($_REQUEST["options"] == "user" and empty($_REQUEST["mailhost"])) {
		$error["mailhost"] = $LANG->_('Missing');
		return false;
	}
	
	// valid
	if($_REQUEST["options"] == "user" and !preg_match("/^([a-z0-9]+\.{1,1}[a-z0-9]+)+$/i",$_REQUEST["mailhost"])) {
		$error["mailhost"] = $LANG->_('Invalid');
		return false;
	}
	
	// check dns if dns option is set
	if($_REQUEST["options"]  == "dns") {
	    $rec = array();			// The names of the found servers.  (Output variable.)
	    $weight = array();		// The weights of the found servers. (Output variable.)
		$res = getmxrr($_REQUEST["mailhost"], $rec, $weight);
		if(count($rec) == 0){
			$error["dns"] = $LANG->_('This domain isn\'t registred within the DNS system') . "<br>";
			return false;
		} else {
		    $mailhost = $rec[0];
		}
	}
	
 	return true;
}
// }}}

// {{{ attribute_init(void)
function attribute_init() {
    global $_pql, $LANG;
	
	// control dns to set options
	$_REQUEST["mailhost"] = $_REQUEST["oldvalue"];

	if(lc($_REQUEST["mailhost"]) == lc($LANG->_('None'))) {
		$_REQUEST["options"] = "none";
		$_REQUEST["oldvalue"] = "";
		$_REQUEST["mailhost"] = "";
	}
}

// }}}

// {{{ attribute_print_form(void)
function attribute_print_form() {
    global $_pql, $error, $LANG;
?>
  <form action="<?php echo $_SERVER["PHP_SELF"]?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left"><?php echo $LANG->_('Mail server')?></th>
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?php echo $LANG->_('Options')?></td>
          <td><input type="radio" name="options" value="dns" <?php if($_REQUEST["options"]=="dns"){ echo "checked"; }?>><?php echo $LANG->_('Standard (DNS entry)')?><input type="radio" name="options" value="user" <?php if($_REQUEST["options"]=="user"){ echo "checked"; }?>><?php echo $LANG->_('User defined')?><input type="radio" name="options" value="none" <?php if($_REQUEST["options"]=="none"){ echo "checked"; }?>><?php echo $LANG->_('None')?></td>
        </tr>

        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?php echo $LANG->_('Host address (FQDN)')?></td>
          <td><?php echo pql_format_error_span($error["mailhost"]); ?><input type="text" name="mailhost" value="<?php echo $_REQUEST["mailhost"]?>"></td>
        </tr>

        <tr class="subtitle">
          <td colspan="2">
            <?php echo pql_format_error_span($error["dns"]); ?>
            <img src="images/info.png" width="16" height="16" alt="" border="0">&nbsp;<?php echo $LANG->_('With option DNS, the first MX record of this domain will be taken')?>
          </td>
        </tr>
      </th>
    </table>

    <input type="hidden" name="submit"   value="1">
    <input type="hidden" name="attrib"   value="<?php echo $_REQUEST["attrib"]?>">
    <input type="hidden" name="oldvalue" value="<?php echo $_REQUEST["oldvalue"]?>">
    <input type="hidden" name="domain"   value="<?php echo urlencode($_REQUEST["domain"])?>">
    <input type="hidden" name="rootdn"   value="<?php echo urlencode($_REQUEST["rootdn"])?>">
    <input type="hidden" name="user"     value="<?php echo urlencode($_REQUEST["user"])?>">
    <input type="hidden" name="view"     value="<?php echo $_REQUEST["view"]?>">
    <br>
    <input type="submit" value="<?php echo $LANG->_('Save')?>">
  </form>
<?php
}
// }}}

// {{{ attribute_save(type)
function attribute_save($type) {
    global $_pql, $LANG;

	switch($type) {
	  case "add":
		// no add operation possible with mail
		break;

	  case "modify":
		if($_REQUEST["mailhost"] == $_REQUEST["oldvalue"]) {
			// don't change if new value equal to old one
			$msg = pql_complete_constant($LANG->_('%what% unchanged'), array('what' => $LANG->_('Mailserver')));
			attribute_forward($msg);
			break;
    	}
		
		if(pql_modify_attribute($_REQUEST["user"], pql_get_define("PQL_ATTR_MAILHOST"),
								'', $_REQUEST["mailhost"])) {
		    if(empty($_REQUEST["mailhost"]))
			  $_REQUEST["mailhost"] = $LANG->_('None');

			$msg = pql_complete_constant($LANG->_('Successfully changed %what% to %new%'),
										 array('what' => $LANG->_('mailserver'),
											   'new'  => $_REQUEST["mailhost"]));
		} else
		  $msg = pql_complete_constant($LANG->_('Failed to change %what%'),
									   array('what' => $LANG->_('mailserver'))) . ": " . pql_format_error(0);
		
		attribute_forward($msg);
		break;
		
	  case "fulldomain":
		// Create a user search filter
		if(pql_get_define("PQL_CONF_USER_FILTER", $_REQUEST["rootdn"]))
		  $filter = pql_get_define("PQL_CONF_USER_FILTER", $_REQUEST["rootdn"]);
		else
		  $filter  = pql_get_define("PQL_CONF_REFERENCE_USERS_WITH", $_REQUEST["rootdn"])."=*";

		// Retreive all users in this branch/domain.
	    $users   = $_pql->get_dn($_REQUEST["domain"], $filter);
		if(is_array($users)) {
			foreach($users as $user) {
				// Retreive the old value from the object
				$old = $_pql->get_attribute($user, pql_get_define("PQL_ATTR_MAILHOST"));
				$return[] = pql_modify_attribute($user, pql_get_define("PQL_ATTR_MAILHOST"),
												 $old, $_REQUEST["mailhost"]);
			}
			
			if(in_array(false, $return))
			  $msg = pql_complete_constant($LANG->_('Failed to change %what%'),
										   array('what' => $LANG->_('mailserver'))).": ".pql_format_error(0);
			else
			  $msg = pql_complete_constant($LANG->_('Successfully changed %what% to %new%'),
										   array('what' => $LANG->_('mailserver'),
												 'new'  => $_REQUEST["mailhost"]));
			
			attribute_forward($msg);
		}
		
	  default:
		die(pql_complete_constant($LANG->_('Unknown save type %type% in file %file%, function save()'),
								  array('type' => $type, 'file' => __FILE__)));
	}
}
// }}}

// Local variables:
// mode: php
// mode: font-lock
// tab-width: 4
// End:
?>
