<?php
// {{{ Get all database suffixes
$attribs = array('namingContexts', 'monitorContext', 'subschemaSubentry');
$database_contexts = pql_get_status($_pql->ldap_linkid, "cn=Databases,".$_pql->ldap_monitor, $attribs);
?>
  <span class="title1"><?php echo $LANG->_('LDAP Server Database Status')?></span>

  <br><br>

  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left"><?php echo $LANG->_('Entry Cache Usage')?>
      <tr>
        <td class="title3"><?php echo $LANG->_('Suffix')?></td>
        <td class="title3"><?php echo $LANG->_('Type')?></td>
      </tr>

<?php if(is_array($database_contexts)) {
		foreach($attribs as $attrib) {
			$attrib = lc($attrib);

			// Get all information about this database/suffix
			if(is_array($database_contexts[$attrib])) {
			  foreach($database_contexts[$attrib] as $context) {
				$database_status = pql_get_status_databases($context);
				if(isset($database_status['monitoredinfo'])) {
?>
      <tr>
        <?php $class=pql_format_table(0); ?>
        <td class="title"><?php echo $context?></td>
        <td class="<?php echo $class?>"><?php echo $database_status['monitoredinfo']?></td>
      </tr>
<?php
				}
			  }
			} else {
			  $database_status = pql_get_status_databases($database_contexts[$attrib]);
			  if(isset($database_status[$attrib])) {
?>
      <tr>
        <?php $class=pql_format_table(0); ?>
        <td class="title"><?php echo $database_status[$attrib]?></td>
        <td class="<?php echo $class?>"><?php echo $database_status['monitoredinfo']?></td>
      </tr>
<?php
			  }
			}
		}
	  } else {
		  // NOTE: Will this ever happen?! Don't think so, but...

		  // Get all information about this database/suffix
		  $database_status = pql_get_status_databases($database_contexts);
		  foreach($database_status as $status) {
?>
      <tr>
        <?php $class=pql_format_table(0); ?>
        <td class="title"><?php echo $status['namingcontexts']?></td>
        <td class="<?php echo $class?>"><?php echo $status['description']?></td>
      </tr>
<?php	  }
	  }
?>
    </th>
  </table>
<?php
// }}}

/*
 * Local variables:
 * mode: php
 * mode: font-lock
 * tab-width: 4
 * End:
 */
?>
