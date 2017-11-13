type item = {
  title: string,
  completed: bool
};

type state = {
  items: list(item)
};

type action = | AddItem;

let component = ReasonReact.reducerComponent("Horse");

let newItem = () => {title: "Another leg", completed: false};

let str = ReasonReact.stringToElement;

let make = (_children) => {
  ...component,
  initialState: () => {
    items: [
      {title: "First leg", completed: false},
      {title: "Second leg", completed: false}
    ]
  },
  reducer: (action, {items}) =>
    switch action {
    | AddItem => ReasonReact.Update({items: [newItem(), ... items]})
  },
  render: ({state: {items}, reduce}) => {
    let numLegs = List.length(items);
    let legString = (numLegs > 1) ? "legs" : "leg";
    <div className="Horse">
      <p className="Horse-name">
        (str("IT IS HORSE"))
      </p>
      <p className="plop">
        (str(string_of_int(numLegs) ++ " " ++ legString))
      </p>
      <button onClick=(reduce((_evt) => AddItem))>
        (str("Do something"))
      </button>
    </div>
  }
};
