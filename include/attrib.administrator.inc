<?php
// attribute plugin for administrator and seealso modification
// $Id: attrib.administrator.inc,v 1.30 2004-02-14 14:25:35 turbo Exp $

require("./left-head.html");

function attribute_check() {
    global $error, $LANG;
	$attrib = $_REQUEST["attrib"];

    // is typed in ?
    if(empty($_REQUEST[$attrib])) {
		$error[$attrib] = $LANG->_('Missing');
		return false;
    }
    
    // don't use ".."
    if(preg_match("/(\.){2,}/", $_REQUEST[$attrib])) {
		$error[$attrib] = $LANG->_('Invalid');
		return false;
    }
    
    return true;
}

function attribute_print_form($action) {
    global $_pql, $error, $LANG;
    global $orgname; 			// Set by domain_edit_attributes.php

	$attrib = $_REQUEST["attrib"];

	if ($action == 'add') $action = 'fulldomain'; // DLW: Talk to Turbo about the relation between $type and $action.

?>
  <form action="<?php echo $_SERVER["PHP_SELF"] ?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
<?php if($attrib == pql_get_define("PQL_GLOB_ATTR_SEEALSO")) { ?>
      <th colspan="3" align="left"><?php echo pql_complete_constant($LANG->_('Add %what% for domain %domain%'), array('what' => $LANG->_('contact person'), 'domain' => $orgname)); ?></th>
<?php } elseif($attrib == pql_get_define("PQL_GLOB_ATTR_EZMLMADMINISTRATOR")) { ?>
      <th colspan="3" align="left"><?php echo pql_complete_constant($LANG->_('Add %what% for domain %domain%'), array('what' => $LANG->_('mailinglist administrator'), 'domain' => $orgname)); ?></th>
<?php } else { ?>
      <th colspan="3" align="left"><?php echo pql_complete_constant($LANG->_('Add %what% for domain %domain%'), array('what' => $LANG->_('domain administrator'), 'domain' => $orgname)); ?></th>
<?php } ?>
        <tr class="<?php pql_format_table(); ?>">
<?php
	if(($attrib == pql_get_define("PQL_GLOB_ATTR_ADMINISTRATOR")) and $_REQUEST["rootdn"] and $_REQUEST["domain"]) {
		// We're should give a user access to THIS branch
?>
          <td class="title"><?=$LANG->_('Administrator DN')?></td>
<?php
	} elseif((($attrib == pql_get_define("PQL_GLOB_ATTR_ADMINISTRATOR")) or
			  ($attrib == pql_get_define("PQL_GLOB_ATTR_EZMLMADMINISTRATOR")))
			 and !$_REQUEST["rootdn"] and !$_REQUEST["domain"] and $_REQUEST["user"]) {
		// We're giving a user access to ('random') branch
?>
          <td class="title"><?=$LANG->_('Branch DN')?></td>
<?php
	} else {
?>
          <td class="title"><?=$LANG->_('User')?></td>
<?php
	}
?>
          <td>
            <?php echo pql_format_error_span($error[$_REQUEST[$attrib]]) . "\n"; ?>
<?php if($action != 'add') { ?>
            <input type="text" name="<?=$attrib?>" value="<?=$_REQUEST[$attrib]?>" size="50"></td>
<?php } else {
		// TODO: This is a REALLY bad idea. If we have a 'huge' database
		// (hundred, thousands or more users), this will take AGES!
		if($_REQUEST["rootdn"] and $_REQUEST["domain"]) {
			// We're giving access to a specific domain
			$i = 0; $j = 0;

			// We're should give a user access to THIS branch	
			foreach($_pql->ldap_basedn as $dn) {
				$dn = urldecode($dn);
				unset($dom); unset($domains);

				$dom = pql_domain_value($_pql, $dn, pql_get_define("PQL_GLOB_ATTR_ADMINISTRATOR"), $_SESSION["USER_DN"]);
				if($dom) {
					foreach($dom as $d) {
						$domains[] = $d;
					}
				}

				if(isset($domains)) {
					asort($domains);
					foreach($domains as $key => $branch) {
						// Get domain part from the DN (Example: 'dc=test,dc=net' => 'test').
						$d = split(',', $branch); $d = split('=', $d[0]); $d = $d[1];
						$d = pql_maybe_decode($d);
?>

<?php if($opera) { // DLW: Is the $opera variable visable here? ?>
            <div id="el2Parent" class="parent" onclick="showhide(el<?=$j?>Spn, el<?=$j?>Img)">
              <img name="imEx" src="images/plus.png" border="0" alt="+" width="9" height="9" id="el<?=$j?>Img">
            </div>
<?php } else { ?>
            <div id="el<?=$j?>Parent" class="parent">
              <a class="item" onClick="if (capable) {expandBase('el<?=$j?>', true); return false;}">
                <img name="imEx" src="images/plus.png" border="0" alt="+" width="9" height="9" id="el<?=$j?>Img">
              </a>
        
              <a class="item">
                <font color="black" class="heada"><?=$d?></font>
              </a>
            </div>
<?php } ?>
        
<?php if($opera) { ?>
            <span id="el<?=$j?>Spn" style="display:'none'">
<?php } else { ?>
            <div id="el<?=$j?>Child" class="child">
<?php } ?>
<?php					// Get all users (their DN) in this domain
						$users = pql_user_get($_pql->ldap_linkid, $branch);
						if(is_array($users)) {
							// Zero out the variables, othervise we won't get users in
							// specified domain, but also in the PREVIOUS domain shown!
							unset($cns);
							foreach($users as $username) {
								$cn = pql_get_attribute($_pql->ldap_linkid, $username, pql_get_define("PQL_GLOB_ATTR_CN"));
								$cns[$username] = $cn[0];
							}
								
							foreach($cns as $userdn => $username) {
?>
              <nobr>&nbsp;&nbsp;&nbsp;&nbsp;<input type="checkbox" name="<?=$attrib?>_<?=$i?>" value="<?=$userdn?>"><?=$username?></nobr><br>
<?php              				$i++;
               				}
						}

						$j++;
?>
<?php if($opera) { ?>
            </span>
<?php } else { ?>
            </div>
<?php } ?>
<?php				}
				}
			}
?>

            <input type="hidden" name="<?=$attrib?>_<?=$i?>" value="on">
            <input type="hidden" name="<?=$attrib?>" value="<?=$i?>">
<?php	} elseif($_REQUEST["user"]) { // We're giving a specific user access to a domain ?>
            <select name="domain">
<?php		if($_SESSION["ALLOW_GLOBAL_CONFIG_SAVE"]) {
				// We're a super-admin, we can give/take
				// adminstration on ANY branch/domain
				// that exists in the system
				$_REQUEST["domains"] = pql_domain_get($_pql);
			} else {
				// We can only give user access to a branch we (the logged in user)
				// have access to. Otherwise, we could 'steal' access to any domain
				// in the system. Only a branch owner/admin can give someone access
				// to the current branch. Am I making myself clear?
				foreach($_pql->ldap_basedn as $dn)  {
					$dn = urldecode($dn);

					$dom = pql_domain_value($_pql, $dn, pql_get_define("PQL_GLOB_ATTR_ADMINISTRATOR"),
											$_SESSION["USER_DN"], 1);
					if($dom)
					  foreach($dom as $d)
						$domains[] = $d;
				}
			}

			if(is_array($domains)) {
				$domains = array_values(array_unique($domains));
				asort($domains);
				foreach($domains as $branch) {
					if($branch == $_pql->ldap_basedn[0]) {
						$shown_first_rootdn = 1;
?>
              <option value="<?=$branch?>">*&nbsp;<?=$branch?></option>
<?php				} else { ?>
              <option value="<?=$branch?>">&nbsp;&nbsp;&nbsp;<?=$branch?></option>
<?php				}
				}
			}
?>
            </select>
<?php	}
	}
?>
<?php if($shown_first_rootdn) { ?>
        <tr class="<?php pql_format_table(); ?>">
          <td><img src="images/info.png" width="16" height="16" alt="" border="0" align="right"></td>
          <td>
            <?=$LANG->_('The DN with the asterisk (*) is the DN where phpQLAdmin configurations are stored. If you add a user to that DN, the user will get \'Super Administration Rights\'')?>!
          </td>
        </tr>
<?php } ?>
      </th>
    </table>

<?php if(!$_REQUEST["user"]) { ?>
    <input type="hidden" name="domain"        value="<?=$_REQUEST["domain"]?>">
<?php } else { ?>
    <input type="hidden" name="<?php echo pql_get_define("PQL_GLOB_ATTR_ADMINISTRATOR");?>" value="<?=$_REQUEST["user"]?>">
    <input type="hidden" name="user"          value="<?=$_REQUEST["user"]?>">
<?php } ?>
    <input type="hidden" name="submit"        value="4">
    <input type="hidden" name="attrib"        value="<?=$_REQUEST["attrib"]?>">
    <input type="hidden" name="action"        value="<?=$action?>">
    <input type="hidden" name="rootdn"        value="<?=$_REQUEST["rootdn"]?>">
    <input type="hidden" name="oldvalue"      value="<?=$_REQUEST[$attrib]?>">
    <input type="hidden" name="view"          value="<?=$_REQUEST["view"]?>">
    <br>
    <input type="submit" value="<?=$LANG->_('Save')?>">
  </form>
<?php
}

/* DLW: This doesn't work, (Default Branch Values/Add domain administrator for domain) but it doesn't work in
 *      the original code either.  Look into it. */
function attribute_save($type) {
    global $_pql, $LANG;

	$attrib = $_REQUEST["attrib"];
	$value = $_REQUEST[$attrib];

    switch($type) {
      case "add":
      case "modify":
      case "delete":
		if($value and $_REQUEST["oldvalue"] and ($value == $_REQUEST["oldvalue"])) {
			// don't change if new value equal to old one
			$msg = pql_complete_constant($LANG->_('%what% unchanged'), array('what' => $LANG->_('Domain administrator')));
			attribute_forward($msg);
			break;
    	}

		// Get the old values, so we can remove the one we're modifying
		// (delete the one we want to delete, keep the others etc)
		if(($_REQUEST["rootdn"] and $_REQUEST["domain"]) or ($_SERVER["user"] and $_REQUEST["domain"]))
		  $admins = pql_domain_value($_pql, $_REQUEST["domain"], $attrib);
		elseif($_SERVER["user"] and $_REQUEST["rootdn"])
		  $admins = pql_domain_value($_pql, $_REQUEST["rootdn"], $attrib);
		elseif($_SERVER["user"] and $value and !$value)
		  $admins = pql_domain_value($_pql, $value, $attrib);

		$ADMs = array();

		if(is_array($admins)) {
			foreach($admins as $admin)
			  $ADMs[] = $admin;
		}

		if($type == 'delete') {
			// Delete attribute
			if(!$_SERVER["user"] and !$value) {
				// Add the new value of administrator
				$ADMs[] = $value;
			} elseif($value) {
				// Delete user DN from array of administrators
				foreach($ADMs as $adm) {
					if(!eregi($value, $adm))
					  $adms[] = $adm;
				}
				
				$ADMs = $adms;
			} else {
				// Delete user DN from array of administrators
				foreach($ADMs as $adm) {
					if(!eregi($_SERVER["user"], $adm))
					  $adms[] = $adm;
				}
				
				$ADMs = $adms;
			}
		} else {
			// Add user 
			if($_SERVER["user"])
			  $ADMs[] = $_SERVER["user"];
			elseif($value) {
				$amount = $value;
				for($i=0; $i < $amount; $i++) {
					$var_name = $attrib . "_$i";
					if($_POST[$var_name]) 
					  $ADMs[] = $_POST[$var_name];
				}
			}
		}

		$success = 0;
		if($_REQUEST["domain"] and (($value and !$_SERVER["user"]) or ($value and $_SERVER["user"]) or (!$value and $_SERVER["user"]))) {
			if(pql_set_domain_value($_pql->ldap_linkid, $_REQUEST["domain"], $attrib, $ADMs))
			  $success = 1;
		} elseif($_REQUEST["rootdn"] and $_SERVER["user"]) {
			if(pql_set_domain_value($_pql->ldap_linkid, $_REQUEST["rootdn"], $attrib, $ADMs))
			  $success = 1;
		}

		if($success)
		  $msg = pql_complete_constant($LANG->_('Successfully changed %what%'),
									   array('what' => $LANG->_('Domain administrator')));
		else
		  $msg = pql_complete_constant($LANG->_('Failed to change %what%'),
									   array('what' => $LANG->_('Domain administrator')))
			. ": " . ldap_error($_pql->ldap_linkid);
		
		attribute_forward($msg);
		break;

      default:
		die(pql_complete_constant($LANG->_('Unknown save type %type% in file %file%, function save()'),
								  array('type' => $type, 'file' => __FILE__)));
    }
}

if(($_REQUEST["attrib"] == pql_get_define("PQL_GLOB_ATTR_ADMINISTRATOR")) or
   ($_REQUEST["attrib"] == pql_get_define("PQL_GLOB_ATTR_SEEALSO")) or
   ($_REQUEST["attrib"] == pql_get_define("PQL_GLOB_ATTR_EZMLMADMINISTRATOR")))
{
	require("./left-trailer.html");
}

// Local variables:
// mode: php
// mode: font-lock
// tab-width: 4
// End:
?>
