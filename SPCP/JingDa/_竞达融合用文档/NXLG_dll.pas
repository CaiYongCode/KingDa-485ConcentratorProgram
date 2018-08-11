

{ 清除卡片数据 }
function ClearCardData(const Port, Baud: Integer): Integer; stdcall;
{ 制作设置卡 }
function MakeSettingCard(const Port, Baud: Integer; const PriceOne, PriceTwo,
  PriceThree, BaseMoney: Double; const PriceOneQuantity, PriceTwoQuantity,
  WarningMoney: Integer): Integer; stdcall;
{ 制作清零卡 }
function MakeCleanAwayCard(const Port, Baud: Integer): Integer; stdcall;
{ 制作检查卡 }
function MakeCheckCard(const Port, Baud: Integer): Integer; stdcall;
{ 制作校时卡 }
function MakeCollateTimeCard(const Port, Baud: Integer;
  SysTime: TDateTime): Integer; stdcall;
{ 制作可修改信息用户卡（开户） }
function MakeSpecialUserCard(const Port, Baud: Integer;
  const BuyTimes, WarningMoney: Integer; const MeterSN: Integer;
  const BuyMoney, PriceOne, PriceTwo, PriceThree, PriceOneQuantity,
  PriceTwoQuantity, BaseMoney, BuyTotal: Double; IsReplace: Integer): Integer;
  stdcall;
{ 制作用户卡 （充值） }
function MakeUserCard(const Port, Baud: Integer; const BuyTimes: Integer;
  const MeterSN: Integer; const BuyMoney: Double): Integer; stdcall;
{ 制作换表卡 }
function MakeChangeMeterCard(const Port, Baud: Integer): Integer; stdcall;
{ 制作抄表卡 }
function MakeReadingCard(const Port, Baud: Integer): Integer; stdcall;
{ 获取卡类型 }
function GetCardType(const Port, Baud: Integer;
  var CardType: Integer): Integer; stdcall;
{ 读取用户卡数据 }
function ReadUserCardData(const Port, Baud: Integer; var BuyTimes,
  PriceOneQuantity, PriceTwoQuantity, MeterState, WarningMoney: Integer;
  var MeterSN: Integer; var BuyMoney, UseTotal, Remain, Overdraw,
  MonthSum12, MonthSum11, MonthSum10, MonthSum9, MonthSum8, MonthSum7,
  MonthSum6, MonthSum5, MonthSum4, MonthSum3, MonthSum2, MonthSum1, PriceOne,
  PriceTwo, PriceThree, BaseMoney, BuyTotal: Double;
  var CutInCardDate: TDateTime): Integer; stdcall;
{ 读取设置卡数据 }
function ReadSettingCardData(const Port, Baud: Integer; var PriceOne, PriceTwo,
  PriceThree, BaseMoney: Double; var PriceOneQuantity, PriceTwoQuantity,
  WarningMoney: Integer): Integer; stdcall;
{ 读取检查卡数据 }
function ReadCheckCardData(const Port, Baud: Integer; var MeterSN: Integer;
  var UseTotal, BuyMoney, Remain, PriceOne, PriceTwo, PriceThree,
  BuyTotal: Double; var PriceOneQuantity, PriceTwoQuantity, UseQuantityTotal,
  MonthSum1, MeterState: Integer): Integer; stdcall;
{ 读取抄表卡数据 }
function ReadReadingCardData(const Port, Baud: Integer; var MeterSN,
  MonthSum1: Integer): Integer; stdcall;
{ 读取校时卡数据 }
function ReadCollateTimeCardData(const Port, Baud: Integer;
  var SysTime: TDateTime): Integer; stdcall;
{ 读取换表卡数据 }
function ReadChangeMeterCardData(const Port, Baud: Integer; var BuyTimes,
  PriceOneQuantity, PriceTwoQuantity, MeterState, WarningMoney: Integer;
  var MeterSN: Integer; var BuyMoney, UseTotal, Remain, Overdraw,
  MonthSum12, MonthSum11, MonthSum10, MonthSum9, MonthSum8, MonthSum7,
  MonthSum6, MonthSum5, MonthSum4, MonthSum3, MonthSum2, MonthSum1, PriceOne,
  PriceTwo, PriceThree, BaseMoney, BuyTotal: Double;
  var CutInCardDate: TDateTime): Integer; stdcall;



