  <!-- Access list -->
  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="2" align="left"><?=$LANG->_('User access')?>
<?php
// {{{ Retreive values
$super_admin = 0;
foreach($_SESSION["BASE_DN"] as $branch) {
  $branch = urldecode($branch);
  
  // {{{ Check the top-most (namingContexts) DN's if user is admin of that...
  if(pql_validate_administrator($_pql->ldap_linkid, $branch, pql_get_define("PQL_ATTR_ADMINISTRATOR"), $_GET["user"])) {
	$domains_admins[$branch][] = $branch;
	$super_admin = 1;
  }
// }}}
  
  // {{{ Find all branches the user is admin of
  $dom = pql_get_dn($_pql->ldap_linkid, $branch, pql_get_define("PQL_ATTR_ADMINISTRATOR").'='.$_GET["user"]);
  if($dom) {
	foreach($dom as $d) {
	  // Filter out the RootDN
	  if($d != $branch)
		$domains_admins[$branch][] = $d;
	}
  }
// }}}
  
  // {{{ Find all branches the user is mailinglist admin of
  $dom = pql_get_dn($_pql->ldap_linkid, $branch, pql_get_define("PQL_ATTR_ADMINISTRATOR_EZMLM").'='.$_GET["user"]);
  if($dom) {
	foreach($dom as $d)
	  $domains_ezmlm[$branch][] = $d;
  }
// }}}

  // {{{ Find all branches the user is webserver admin of
  $dom = pql_get_dn($_pql->ldap_linkid, $branch, pql_get_define("PQL_ATTR_ADMINISTRATOR_WEBSRV").'='.$_GET["user"]);
  if($dom) {
	foreach($dom as $d)
	  $domains_websrv[$branch][] = $d;
  }
// }}}

  // {{{ Find all branches the user is DNS admin of
  $dom = pql_get_dn($_pql->ldap_linkid, $branch, pql_get_define("PQL_ATTR_ADMINISTRATOR_DNS").'='.$_GET["user"]);
  if($dom) {
	foreach($dom as $d)
	  $domains_dns[$branch][] = $d;
  }
// }}}
}
// }}}


// {{{ Go through and show all the domains that the user is administrator of
$class=pql_format_table(0);
if(isset($domains_admins)) {
  asort($domains_admins);
  
  $new_tr = 0;
  foreach($domains_admins as $branch => $doms) {
	foreach($doms as $branch) {
	  $branch = urlencode($branch);
	  
	  if($new_tr) {
?>

      <tr class="<?=$class?>">
        <td class="title"></td>
<?php } else { ?>
      <tr class="<?=$class?>">
        <td class="title"><?=$LANG->_('Administrator in branch')?>:</td>
<?php } $new_tr = 1; ?>
        <td><a href="domain_detail.php?rootdn=<?=$branch?>&domain=<?=$branch?>"><?=pql_maybe_decode(urldecode($branch))?></a></td>
        <td>
          <a href="domain_edit_attributes.php?attrib=<?=pql_get_define("PQL_ATTR_ADMINISTRATOR")?>&rootdn=<?=$branch?>&domain=<?=$branch?>&branchdn=<?=$branch?>&administrator=<?=$url["user"]?>&submit=4&action=delete&view=<?=$_GET["view"]?>"><img src="images/del.png" width="12" height="12" border="0" alt="<?=$LANG->_('Deny user admin access to domain')?>"></a>
        </td>
      </tr>
<?php
	}
  }
?>
      <tr class="<?=$class?>">
        <td class="title"></td>
        <td>
          <a href="domain_edit_attributes.php?attrib=<?=pql_get_define("PQL_ATTR_ADMINISTRATOR")?>&rootdn=<?=$url["rootdn"]?>&domain=<?=$branch?>&user=<?=$url["user"]?>&submit=3&action=add&view=<?=$_GET["view"]?>"><?=$LANG->_('Give (more) admin rights')?></a>
        </td>
        <td></td>
      </tr>

<?php
} else {
?>
      <tr class="<?=$class?>">
        <td class="title"><?=$LANG->_('Administrator in branch')?>:</td>
        <td>
          <a href="domain_edit_attributes.php?attrib=<?=pql_get_define("PQL_ATTR_ADMINISTRATOR")?>&rootdn=<?=$url["rootdn"]?>&domain=<?=$branch?>&user=<?=$url["user"]?>&submit=3&action=add&view=<?=$_GET["view"]?>"><?=$LANG->_('Give admin rights')?></a>
        </td>
        <td></td>
      </tr>
<?php
}
// }}}

// {{{ Go through and show all the domains that the user is MAILINGLIST administrator of
$class=pql_format_table(0);
if(isset($domains_ezmlm)) {
  asort($domains_ezmlm);
  
  $new_tr = 0;
  foreach($domains_ezmlm as $branch => $doms) {
	foreach($doms as $branch) {
	  $branch = urlencode($branch);
	  
	  if($new_tr) {
?>

      <tr class="<?=$class?>">
        <td class="title"></td>
<?php } else { ?>
      <tr class="<?=$class?>">
        <td class="title"><?=$LANG->_('Mailinglist administrator in branch')?>:</td>
<?php } $new_tr = 1; ?>
        <td><a href="domain_detail.php?rootdn=<?=$branch?>&domain=<?=$branch?>"><?=pql_maybe_decode(urldecode($branch))?></a></td>
        <td>
          <a href="domain_edit_attributes.php?attrib=<?=pql_get_define("PQL_ATTR_ADMINISTRATOR_EZMLM")?>&rootdn=<?=$branch?>&domain=<?=$branch?>&branchdn=<?=$branch?>&administrator=<?=$url["user"]?>&submit=4&action=delete&view=<?=$_GET["view"]?>"><img src="images/del.png" width="12" height="12" border="0" alt="<?=$LANG->_('Deny user admin access to domain')?>"></a>
        </td>
      </tr>
<?php
	}
  }
?>
      <tr class="<?=$class?>">
        <td class="title"></td>
        <td>
          <a href="domain_edit_attributes.php?attrib=<?=pql_get_define("PQL_ATTR_ADMINISTRATOR_EZMLM")?>&rootdn=<?=$url["rootdn"]?>&domain=<?=$branch?>&user=<?=$url["user"]?>&submit=3&action=add&view=<?=$_GET["view"]?>"><?=$LANG->_('Give (more) admin rights')?></a>
        </td>
        <td></td>
      </tr>
<?php
} else {
?>
      <tr class="<?=$class?>">
        <td class="title"><?=$LANG->_('Mailinglist administrator in branch')?>:</td>
        <td>
          <a href="domain_edit_attributes.php?attrib=<?=pql_get_define("PQL_ATTR_ADMINISTRATOR_EZMLM")?>&rootdn=<?=$url["rootdn"]?>&domain=<?=$branch?>&user=<?=$url["user"]?>&submit=3&action=add&view=<?=$_GET["view"]?>"><?=$LANG->_('Give admin rights')?></a>
        </td>
        <td></td>
      </tr>
<?php
}
// }}}

// {{{ Go through and show all the domains that the user is WEBSERVER administrator of
$class=pql_format_table(0);
if(isset($domains_websrv)) {
  asort($domains_websrv);
  
  $new_tr = 0;
  foreach($domains_websrv as $branch => $doms) {
	foreach($doms as $branch) {
	  $branch = urlencode($branch);
	  
	  if($new_tr) {
?>

      <tr class="<?=$class?>">
        <td class="title"></td>
<?php } else { ?>
      <tr class="<?=$class?>">
        <td class="title"><?=$LANG->_('Webserver administrator in branch')?>:</td>
<?php } $new_tr = 1; ?>
        <td><a href="domain_detail.php?rootdn=<?=$branch?>&domain=<?=$branch?>"><?=pql_maybe_decode(urldecode($branch))?></a></td>
        <td>
          <a href="domain_edit_attributes.php?attrib=<?=pql_get_define("PQL_ATTR_ADMINISTRATOR_WEBSRV")?>&rootdn=<?=$branch?>&domain=<?=$branch?>&branchdn=<?=$branch?>&<?=pql_get_define("PQL_ATTR_ADMINISTRATOR_WEBSRV")?>=<?=$url["user"]?>&submit=4&action=delete&view=<?=$_GET["view"]?>"><img src="images/del.png" width="12" height="12" border="0" alt="<?=$LANG->_('Deny user admin access to domain')?>"></a>
        </td>
      </tr>
<?php
	}
  }
?>
      <tr class="<?=$class?>">
        <td class="title"></td>
        <td>
          <a href="domain_edit_attributes.php?attrib=<?=pql_get_define("PQL_ATTR_ADMINISTRATOR_WEBSRV")?>&rootdn=<?=$url["rootdn"]?>&domain=<?=$branch?>&user=<?=$url["user"]?>&submit=3&action=add&view=<?=$_GET["view"]?>"><?=$LANG->_('Give (more) admin rights')?></a>
        </td>
        <td></td>
      </tr>
<?php
} else {
?>
      <tr class="<?=$class?>">
        <td class="title"><?=$LANG->_('Webserver administrator in branch')?>:</td>
        <td>
          <a href="domain_edit_attributes.php?attrib=<?=pql_get_define("PQL_ATTR_ADMINISTRATOR_WEBSRV")?>&rootdn=<?=$url["rootdn"]?>&domain=<?=$branch?>&user=<?=$url["user"]?>&submit=3&action=add&view=<?=$_GET["view"]?>"><?=$LANG->_('Give admin rights')?></a>
        </td>
        <td></td>
      </tr>
<?php
}
// }}}

// {{{ Go through and show all the domains that the user is DNS administrator of
$class=pql_format_table(0);
if(isset($domains_dns)) {
  asort($domains_dns);
  
  $new_tr = 0;
  foreach($domains_dns as $branch => $doms) {
	foreach($doms as $branch) {
	  $branch = urlencode($branch);
	  
	  if($new_tr) {
?>

      <tr class="<?=$class?>">
        <td class="title"></td>
<?php } else { ?>
      <tr class="<?=$class?>">
        <td class="title"><?=$LANG->_('DNS administrator in branch')?>:</td>
<?php } $new_tr = 1; ?>
        <td><a href="domain_detail.php?rootdn=<?=$branch?>&domain=<?=$branch?>"><?=pql_maybe_decode(urldecode($branch))?></a></td>
        <td>
          <a href="domain_edit_attributes.php?attrib=<?=pql_get_define("PQL_ATTR_ADMINISTRATOR_DNS")?>&rootdn=<?=$branch?>&domain=<?=$branch?>&branchdn=<?=$branch?>&<?=pql_get_define("PQL_ATTR_ADMINISTRATOR_DNS")?>=<?=$url["user"]?>&submit=4&action=delete&view=<?=$_GET["view"]?>"><img src="images/del.png" width="12" height="12" border="0" alt="<?=$LANG->_('Deny user admin access to domain')?>"></a>
        </td>
      </tr>
<?php
	}
  }
?>
      <tr class="<?=$class?>">
        <td class="title"></td>
        <td>
          <a href="domain_edit_attributes.php?attrib=<?=pql_get_define("PQL_ATTR_ADMINISTRATOR_DNS")?>&rootdn=<?=$url["rootdn"]?>&domain=<?=$branch?>&user=<?=$url["user"]?>&submit=3&action=add&view=<?=$_GET["view"]?>"><?=$LANG->_('Give (more) admin rights')?></a>
        </td>
        <td></td>
      </tr>
<?php
} else {
?>
      <tr class="<?=$class?>">
        <td class="title"><?=$LANG->_('DNS administrator in branch')?>:</td>
        <td>
          <a href="domain_edit_attributes.php?attrib=<?=pql_get_define("PQL_ATTR_ADMINISTRATOR_DNS")?>&rootdn=<?=$url["rootdn"]?>&domain=<?=$branch?>&user=<?=$url["user"]?>&submit=3&action=add&view=<?=$_GET["view"]?>"><?=$LANG->_('Give admin rights')?></a>
        </td>
        <td></td>
      </tr>
<?php
}
// }}}


// {{{ Are the user allowed to create branches (i.e. is the user listed in the Root DN)?
if($_SESSION["ALLOW_BRANCH_CREATE"]) {
  $class=pql_format_table(0);
?>
      <tr class="<?=$class?>">
        <td class="title"><?=$LANG->_('Create branches')?></td>
<?php	if($super_admin) { ?>
        <td><?=$LANG->_('Yes')?></td>
        <td>
          <a href="domain_edit_attributes.php?attrib=<?=pql_get_define("PQL_ATTR_ADMINISTRATOR")?>&rootdn=<?=$url["rootdn"]?>&domain=<?=$url["domain"]?>&user=<?=$url["user"]?>&submit=4&<?=pql_get_define("PQL_ATTR_ADMINISTRATOR")?>=<?=$url["user"]?>&action=delete&view=<?=$_GET["view"]?>">
            <img src="images/del.png" width="12" height="12" border="0" alt="<?php echo pql_complete_constant($LANG->_('Disallow user to create domains below %domain%'), array('domain' => $_GET["rootdn"])); ?>">
          </a>
        </td>
<?php } else { ?>
        <td><?=$LANG->_('No')?></td>
        <td>
          <a href="domain_edit_attributes.php?attrib=<?=pql_get_define("PQL_ATTR_ADMINISTRATOR")?>&rootdn=<?=$url["rootdn"]?>&domain=<?=$url["domain"]?>&user=<?=$url["user"]?>&submit=4&action=add&view=<?=$_GET["view"]?>"><img src="images/edit.png" width="12" height="12" border="0" alt="<?php echo pql_complete_constant($LANG->_('Allow user to create domains below %domain%'), array('domain' => $_GET["domain"])); ?>">
          </a>
        </td>
<?php   } ?>
      </tr>

<?php
}
// }}}

// {{{ Are the user allowed to administrate mailserver information?
?>
      <tr class="<?=$class?>">
        <td class="title"><?=$LANG->_('Administrate mailservers')?></td>
<?php if($controlsadministrator) { ?>
        <td><?=$LANG->_('Yes')?></td>
        <td>
          <a href="config_edit_attribute.php?rootdn=<?=$url["rootdn"]?>&domain=<?=$url["domain"]?>&attrib=<?=pql_get_define("PQL_ATTR_ADMINISTRATOR_CONTROLS")?>&action=del&<?=pql_get_define("PQL_ATTR_ADMINISTRATOR_CONTROLS")?>=<?=$url["user"]?>&view=<?=$_GET["view"]?>"><img src="images/del.png" width="12" height="12" border="0" alt="<?=$LANG->_('Disallow user to administrate mailserver information')?>"></a>
        </td>
<?php } else {
		if(@$super_admin) {
?>
        <td><?=$LANG->_('Yes (user is superadmin)')?></td>
<?php	} else {
?>
        <td><?=$LANG->_('No')?></td>
<?php	} ?>
        <td>
<?php	if(!@$super_admin) { ?>
          <a href="config_edit_attribute.php?rootdn=<?=$url["rootdn"]?>&domain=<?=$url["domain"]?>&attrib=<?=pql_get_define("PQL_ATTR_ADMINISTRATOR_CONTROLS")?>&action=del&<?=pql_get_define("PQL_ATTR_ADMINISTRATOR_CONTROLS")?>=<?=$url["user"]?>&view=<?=$_GET["view"]?>"><img src="images/edit.png" width="12" height="12" border="0" alt="<?=$LANG->_('Allow user to administrate mailserver information')?>"></a>
<?php	} ?>
        </td>
<?php } ?>
      </tr>

<?php
// }}}


// {{{ When user do a login, should the advanced mode be loaded?
?>
      <tr class="<?=$class?>">
        <td class="title"><?=$LANG->_('Start in advanced mode')?></td>
<?php if($startwithadvancedmode) { ?>
        <td><?=$LANG->_('Yes')?></td>
        <td>
          <a href="user_edit_attribute.php?rootdn=<?=$url["rootdn"]?>&domain=<?=$url["domain"]?>&user=<?=$url["user"]?>&attrib=<?=pql_get_define("PQL_ATTR_START_ADVANCED")?>&oldvalue=1&view=<?=$_GET["view"]?>"><img src="images/del.png" width="12" height="12" border="0" alt="<?=$LANG->_('Do not start in advanced mode')?>"></a>
        </td>
<?php } else { ?>
        <td><?=$LANG->_('No')?></td>
        <td>
          <a href="user_edit_attribute.php?rootdn=<?=$url["rootdn"]?>&domain=<?=$url["domain"]?>&user=<?=$url["user"]?>&attrib=<?=pql_get_define("PQL_ATTR_START_ADVANCED")?>&oldvalue=0&view=<?=$_GET["view"]?>"><img src="images/edit.png" width="12" height="12" border="0" alt="<?=$LANG->_('Make sure we start in advanced mode')?>"></a>
        </td>
<?php } ?>
      </tr>
<?php
// }}}
?>
    </th>
  </table>

  <br><br>
<?php
/*
 * Local variables:
 * mode: php
 * mode: font-lock
 * tab-width: 4
 * End:
 */
?>
