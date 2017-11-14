type item = {
  id: int,
  title: string,
  completed: bool
};


let str = ReasonReact.stringToElement;

module Leg = {
  let component = ReasonReact.statelessComponent("Leg");
  let make = (~item, ~onToggle, _children) => {
    ...component,
    render: (_self) =>
      <div className="leggy" onClick=((_evt) => onToggle())>
        <input
          _type="checkbox"
          checked=(Js.Boolean.to_js_boolean(item.completed))
          /* make interactive */
        />
        (str(item.title))
      </div>
  };
};
