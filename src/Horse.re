
type item = {
  id: int,
  title: string,
  completed: bool
};

type repo = {
  full_name: string,
  stargazers_count: int,
  html_url: string
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

let valueFromEvent = (evt) : string => (
  evt
  |> ReactEventRe.Form.target
  |> ReactDOMRe.domElementToObj
)##value;

module Input = {
  type state = string;
  let component = ReasonReact.reducerComponent("Input");
  let make = (~onSubmit, _) => {
    ...component,
    initialState: () => "",
    reducer: (newText, _text) => ReasonReact.Update(newText),
    render: ({state: text, reduce}) =>
      <input
        value=text
        _type="text"
        placeholder="Write something to do"
        onChange=(reduce((evt) => valueFromEvent(evt)))
        onKeyDown=((evt) =>
          if (ReactEventRe.Keyboard.key(evt) == "Enter") {
            onSubmit(text);
            (reduce(() => ""))()
          }
        )
      />
  };
};

type state = {
  items: list(item)
};

type action =
  | AddItem(string)
  | ToggleItem(int);

let component = ReasonReact.reducerComponent("Horse");
let lastId = ref(0);
let newItem = (text) => {
  lastId := lastId^ + 1;
  {id: lastId^, title: text, completed: false}
};

let make = (_children) => {
  ...component,
  initialState: () => {
    items: [
      {
        id:0,
        title: "First leg",
        completed: false
      }
    ]
  },
  didMount: (_self) => {
    
    let parseRepoJson = (json: Js.Json.t): repo => 
      Json.Decode.{
        full_name: field("full_name", string, json),
        stargazers_count: field("stargazers_count", int, json),
        html_url: field("html_url", string, json)
      };
    
    let parseReposResponseJson = json =>
      Json.Decode.(field("items", array(parseRepoJson), json));
    
    let reposUrl = "https://api.github.com/search/repositories?q=topic%3Areasonml&type=Repositories";
    
    let fetchRepos = () =>
      Js.Promise.(
        Bs_fetch.fetch(reposUrl)
          |> then_(Bs_fetch.Response.text)
          |> then_(
            jsonText =>
              resolve(parseReposResponseJson(Js.Json.parseExn(jsonText)))
          )
      );

    fetchRepos()
    |> Js.Promise.then_(repoData => {
        Js.log(repoData);
        Js.Promise.resolve();
      })
    |> ignore;
    ReasonReact.NoUpdate;
  },
  reducer: (action, {items}) =>
    switch action {
    | AddItem(text) => ReasonReact.Update({items: [newItem(text), ... items]})
    | ToggleItem(id) =>
      let items = List.map(
        (item) =>
          item.id === id ?
            {...item, completed: ! item.completed} : item,
          items
      );
      ReasonReact.Update({items: items})
  },
  render: ({state: {items}, reduce}) => {
    let numLegs = List.length(items);
    let legString = (numLegs > 1) ? "legs" : "leg";
    <div className="Horse">
      <p className="Horse-name">
        (str("IT IS HORSE"))
        <Input onSubmit=(reduce((text) => AddItem(text))) />
      </p>
      <div className="Legs">
        (
          List.map(item => <Leg
            key=(string_of_int(item.id))
            onToggle=(reduce(() => ToggleItem(item.id)))
            item
          />, items) |> Array.of_list |> ReasonReact.arrayToElement
        )
      </div>

      <p className="numberOfLegs">
        (str(string_of_int(numLegs) ++ " " ++ legString))
      </p>
    </div>
  }
};
