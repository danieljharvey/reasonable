let component = ReasonReact.statelessComponent("Horse");

let make = (~message, _children) => {
  ...component,
  render: (_self) =>
    <div className="Horse">
      <p className="Horse-name">
        (ReasonReact.stringToElement("It is a horse"))
      </p>
    </div>
};
