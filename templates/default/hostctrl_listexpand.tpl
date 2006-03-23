  <div id='{$show.role}.{$show.attribute}Parent' class='parent'>
    <a href="{$smarty.server.REQUEST_URI}"
       onClick="if (capable) {expandBase('{$show.role}.{$show.attribute}', true); return false;}">
       <img name='imEx' src='images/plus.png' border='0' alt='+' width='9' height='9' id='{$show.role}.{$show.attribute}Img'>
    </a>
    {$show.data.0}
    <a href="{$smarty.server.REQUEST_URI}?{$show.url}&{$show.attribute}={$show.data.0}?>">
      <img src='images/del.png' width='12' height='12' border='0' alt="{$show.alt}"></a><br>
  </div>

  <div id='{$show.role}.{$show.attribute}Child' class='child'>
    <nobr>
{foreach item=line from=$show.data}
      {$line}
      <a href="{$smarty.server.REQUEST_URI}?{$show.url}&attribute={$show.attribute}&{$show.attribute}={$line}">
        <img src='images/del.png' width='12' height='12' border='0' alt="{$show.alt}"></a><br>
{/foreach}
    </nobr>
  </div>";
